#include <iostream>
#include "Session.hpp"

using namespace Networking;

Session::Session(boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket) noexcept
	:_socket{ std::move(socket) }
{
	_data.resize(maxLength);
	_receiveData.reserve(maxLength);
}

void Session::start()
{
	do_handshake();
}

void Session::do_handshake() 
{
	auto self{ shared_from_this() };

	_socket.async_handshake(boost::asio::ssl::stream_base::server,
		[self](const boost::system::error_code& ec) {
			if (!ec)
			{
				self->do_read();
			}	
		
		});
}

void Session::do_read()
{
	auto self{ shared_from_this() };

	_socket.async_read_some(boost::asio::buffer(_data, maxLength), [self]
	(boost::system::error_code ec, size_t length) {
			if (!ec)
			{
				// read the whole message
				// add the data to receive data
				self->_receiveData.insert(self->_receiveData.end(), self->_data.begin(), self->_data.end());

				// Get the metadata
				self->receivedLength += length;
				if (self->receivedLength > Header::headerSize - 1 && self->_messageSize == 0)
				{
					std::memcpy(&self->_messageSize, self->_receiveData.data() + 2, sizeof(uint64_t));
					self->_messageSize += Header::headerSize;
				}

				if (self->receivedLength == self->_messageSize)
				{
					// finished reading echo the message back
					self->do_write();
				}
				else
				{
					// read some more
					self->do_read();
				}
			}
			else if (ec == boost::asio::error::connection_reset)
			{
				WARN("Client aborted the connection.");
			}
			else
			{
				ERROR(ec.message());
			}
		});
}

void Session::do_write()
{
	auto self{ shared_from_this() };


	boost::asio::async_write(_socket, boost::asio::buffer(_receiveData, receivedLength), [this, self]
	(boost::system::error_code ec, size_t length)
		{
			if (!ec)
			{
				// reset everything ready for the next message
				self->_messageSize = 0;
				self->receivedLength = 0;
				self->_receiveData.clear();
				INFO("Wrote: " + std::to_string( length ) + " bytes");
				do_read();
			}
			else if (ec == boost::asio::error::connection_reset)
			{
				WARN("Client aborted the connection.");
			}
			else
			{
				ERROR(ec.message());
			}
		});
}