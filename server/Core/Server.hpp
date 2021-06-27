#pragma once

#include <boost/asio.hpp>

class Server
{
public:
	Server(boost::asio::io_context* ctx, unsigned short port) noexcept;
	void start();

private:
	void do_accept();

private:
	boost::asio::ip::tcp::acceptor _acceptor;
};