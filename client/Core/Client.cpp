#include <iostream>
#include <algorithm>
#include <chrono>
#include "Client.hpp"

using namespace Networking;

Client::Client(boost::asio::io_context* ctx, const std::string& host
	, const std::string& port) noexcept
	:_context{ boost::asio::ssl::context::tlsv13_client },
	_socket{ *ctx, _context }, _resolver{*ctx},
	_host{host}, _port{port},
	_deadline{*ctx}
{
	_data.resize(maxLength);
	_receiveData.reserve(maxLength);

	_context.load_verify_file("E:/pawel/coding(learning)/c++/chatApp/certs/csr.pem");
	_context.set_verify_mode(boost::asio::ssl::verify_peer);
	_socket.set_verify_callback(std::bind(&Client::verify_certificate, this, std::placeholders::_1, std::placeholders::_2));
}

bool Client::verify_certificate(bool preverified, boost::asio::ssl::verify_context& ctx)
{
	char subject_name[256];
	X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
	X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
	INFO(std::string{ "Verifying " + std::string{subject_name} });
	return preverified;
}

void Client::connect()
{
	auto self{ shared_from_this() };
	boost::system::error_code ec;
	tcp::resolver::results_type results = _resolver.resolve(_host, _port, ec);
	if (!ec)
	{
		_deadline.expires_after(std::chrono::seconds(expiryTime));
		boost::asio::async_connect(_socket.lowest_layer(), results, [this, self]
		(const boost::system::error_code& ec, const boost::asio::ip::tcp::endpoint& ep) {
				if (_stopped)
					return;

				if (_socket.lowest_layer().is_open())
				{
					if (!ec)
					{
						handshake();
					}
					else
					{
						ERROR(ec.message());
						stop();
					}
				}
				else
				{
					ERROR("Failed to open a socket");
					stop();
				}
				
			});
		_deadline.async_wait(std::bind(&Client::check_deadline, this));
	}
	else
	{
		ERROR(ec.message());
		stop();
	}
}

void Client::handshake() 
{
	_socket.async_handshake(boost::asio::ssl::stream_base::client,
		[this](const boost::system::error_code& ec) {
			if (!ec)
			{
				// Start full duplex interaction with the server
				do_write();
				do_read();
			}
			else 
			{
				ERROR("Handshake failed");
			}
		});
}

void Client::do_write()
{
	// get the latest message in the queue
	DataTypes::CommunicateData writeData = _writeQueue.pop();

	_deadline.expires_after(std::chrono::seconds(expiryTime));
	auto self{ shared_from_this() };

	// add the necessary metadata
	uint8_t* data = Header::addHeader(writeData._data->Serialize(), writeData._size, Header::MessageType::plainText);

	boost::asio::async_write(_socket, boost::asio::buffer(data, writeData._size + Header::headerSize), 
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
				ERROR(ec.message());
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
				if (self->receivedLength > Header::headerSize - 1 && self->_messageSize == 0)
				{
					std::memcpy(&self->_messageSize, self->_receiveData.data() + 2, sizeof(uint64_t));
					self->_messageSize += Header::headerSize;
				}

				if (self->receivedLength == self->_messageSize)
				{
					// add the message to the receiveQueue
					self->receivedLength -= Header::headerSize;

					// add message to receive queue without the header
					std::shared_ptr<std::vector<uint8_t>> tmp = std::make_shared<std::vector<uint8_t>>(std::move(self->_receiveData));
					self->_receiveQueue.push(DataTypes::CommunicateData<std::vector<uint8_t>>{ tmp , self->receivedLength});

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
				ERROR(ec.message());
				self->stop();
			}
		
		});
}

void Client::stop()
{
	_stopped = true;
	boost::system::error_code ignored_error;
	_socket.lowest_layer().close(ignored_error);
	_deadline.cancel();
}

void Client::check_deadline()
{
	if (_stopped)
		return;

	if (_deadline.expiry() <= boost::asio::steady_timer::clock_type::now())
	{
		ERROR("Timeout");
		_stopped = true;
		boost::system::error_code ignored_error;
		_socket.lowest_layer().close(ignored_error);
	}
	else
	{
		_deadline.async_wait(std::bind(&Client::check_deadline, this));
	}
}