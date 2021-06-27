#include <iostream>
#include <algorithm>
#include <chrono>
#include "Client.hpp"

Client::Client(boost::asio::io_context* ctx, const std::string& host
	, const std::string& port) noexcept
	:_socket{ *ctx }, _resolver{*ctx},
	_host{host}, _port{port},
	_deadline{*ctx}
{
	_data.resize(maxLength);
	_receiveData.reserve(maxLength);
}


void Client::connect()
{
	auto self{ shared_from_this() };
	boost::system::error_code ec;
	tcp::resolver::results_type results = _resolver.resolve(_host, _port, ec);
	if (!ec)
	{
		_deadline.expires_after(std::chrono::seconds(expiryTime));
		boost::asio::async_connect(_socket, results, [this, self]
		(const boost::system::error_code& ec, const boost::asio::ip::tcp::endpoint& ep) {
				if (_stopped)
					return;

				if (_socket.is_open())
				{
					if (!ec)
					{
						// Start full duplex interaction with the server
						do_write();
						do_read();
					}
					else
					{
						std::cerr << ec.message() << "\n";
						stop();
					}
				}
				else
				{
					std::cout << "Failed to open a socket\n";
					stop();
				}
				
			});
		_deadline.async_wait(std::bind(&Client::check_deadline, this));
	}
	else
	{
		std::cerr << ec.message() << "\n";
		stop();
	}
}

void Client::do_write()
{
	// get the latest message in the queue
	common::CommunicateData writeData = _writeQueue.pop();

	_deadline.expires_after(std::chrono::seconds(expiryTime));
	auto self{ shared_from_this() };

	// add the necessary metadata
	uint8_t* data = common::Utils::addHeader(writeData._data.get(), writeData._size, common::MessageType::plainText);

	boost::asio::async_write(_socket, boost::asio::buffer(data, writeData._size + common::headerSize), 
	[self, data] (const boost::system::error_code& ec, size_t length)
		{
			if (self->_stopped)
				return;

			if (!ec)
			{
				delete[] data; // release the buffer
				self->do_write();
			}
			else
			{
				std::cerr << ec.message() << "\n";
				self->stop();
			}
		});
}

void Client::do_read()
{
	auto self{ shared_from_this() };
	_deadline.expires_after(std::chrono::seconds(expiryTime));

	_socket.async_read_some(boost::asio::buffer(_data, maxLength), [self]
	(boost::system::error_code ec, size_t length)
		{
			if (self->_stopped)
				return;

			if (!ec)
			{
				// add the data to receive data
				self->_receiveData.insert(self->_receiveData.end(), self->_data.begin(), self->_data.end());

				// Get the metadata
				self->receivedLength += length;
				if (self->receivedLength > 5 && self->_messageSize == 0)
				{
					std::memcpy(&self->_messageSize, self->_receiveData.data() + 2, sizeof(uint64_t));
					self->_messageSize += common::headerSize;
				}

				if (self->receivedLength == self->_messageSize)
				{
					// add the message to the receiveQueue
					self->receivedLength -= common::headerSize;

					// add message to receive queue without the header
					self->_receiveQueue.push(common::CommunicateData{ self->_receiveData.data() + common::headerSize, self->receivedLength});

					// reset everything ready for the next message
					self->_messageSize = 0;
					self->receivedLength = 0;
					self->_receiveData.clear();
				}

				// read some more
				self->do_read();
			}
			else
			{
				std::cerr << ec.message() << "\n";
				self->stop();
			}
		
		});
}

void Client::stop()
{
	_stopped = true;
	boost::system::error_code ignored_error;
	_socket.close(ignored_error);
	_deadline.cancel();
}

void Client::check_deadline()
{
	if (_stopped)
		return;

	if (_deadline.expiry() <= boost::asio::steady_timer::clock_type::now())
	{
		std::cout << "Expired\n";
		_stopped = true;
		boost::system::error_code ignored_error;
		_socket.close(ignored_error);
	}
	else
	{
		_deadline.async_wait(std::bind(&Client::check_deadline, this));
	}
}