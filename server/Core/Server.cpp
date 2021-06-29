#include "Server.hpp"
#include "Session.hpp"
#include <memory>

Server::Server(boost::asio::io_context* ctx, unsigned short port) noexcept
	:_acceptor{ *ctx, boost::asio::ip::tcp::endpoint{boost::asio::ip::tcp::v4(), port}},
	_context{boost::asio::ssl::context::tlsv13_server }
{
	_context.set_options(
		boost::asio::ssl::context::default_workarounds |
		boost::asio::ssl::context::no_sslv2 |
		boost::asio::ssl::context::no_sslv3
	);

	_context.use_certificate_file("E:/pawel/coding(learning)/c++/chatApp/certs/cert.pem", boost::asio::ssl::context_base::file_format::pem);
	_context.use_private_key_file("E:/pawel/coding(learning)/c++/chatApp/certs/key.pem", boost::asio::ssl::context_base::file_format::pem);
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
				std::make_shared<Session>(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>(std::move(_socket), _context))->start();
			}
			
			do_accept();
		});
}