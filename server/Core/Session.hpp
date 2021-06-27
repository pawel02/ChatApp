#pragma once
#include <memory>
#include <boost/asio.hpp>

/*
At the moment this is a dumb echo server
*/
class Session : public std::enable_shared_from_this<Session>
{

public:
	Session(boost::asio::ip::tcp::socket socket) noexcept;

	void start();

private:
	void do_read();
	void do_write(size_t length);

private:
	enum { maxLength = 1024 };
	uint8_t data[maxLength];

	boost::asio::ip::tcp::socket _socket;
};