#include "Server.hpp"
#include "Session.hpp"
#include <memory>

Server::Server(boost::asio::io_context* ctx, unsigned short port) noexcept
	:_acceptor{ *ctx, boost::asio::ip::tcp::endpoint{boost::asio::ip::tcp::v4(), port}}
{
}

void Server::start()
{
	do_accept();
}

void Server::do_accept()
{
	_acceptor.async_accept([this](boost::system::error_code ec, boost::asio::ip::tcp::socket _socket)
		{
			if (!ec)
			{
				std::make_shared<Session>(std::move(_socket))->start();
			}
			
			do_accept();
		});
}