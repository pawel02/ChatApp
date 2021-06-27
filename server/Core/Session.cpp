#include <iostream>
#include "Session.hpp"

Session::Session(boost::asio::ip::tcp::socket socket) noexcept
	:_socket{ std::move(socket) }
{}

void Session::start()
{
	do_read();
}

void Session::do_read()
{
	auto self{ shared_from_this() };

	_socket.async_read_some(boost::asio::buffer(data, maxLength), [this, self]
	(boost::system::error_code ec, size_t length) {
			std::cout << length << "\n";
			if (!ec)
			{
				// simply echo back the message for now
				do_write(length);
			}
			else if (ec == boost::asio::error::connection_reset)
			{
				std::cout << "Client aborted the connection.\n";
			}
			else
			{
				std::cout << ec.message() << "\n";
			}
		});
}

void Session::do_write(size_t length)
{
	auto self{ shared_from_this() };
	boost::asio::async_write(_socket, boost::asio::buffer(data, length), [this, self]
	(boost::system::error_code ec, size_t length)
		{
			if (!ec)
			{
				do_read();
			}
			else if (ec == boost::asio::error::connection_reset)
			{
				std::cout << "Client aborted the connection.\n";
			}
			else
			{
				std::cout << ec.message() << "\n";
			}
		});
}