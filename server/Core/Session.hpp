#pragma once
#include <memory>
#include <boost/asio.hpp>

#include <CommonLib.hpp>

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
	void do_write();

private:
	enum { maxLength = 1024 };
	uint8_t data[maxLength];

	Networking::DataTypes::sizeType _messageSize = 0;
	Networking::DataTypes::sizeType receivedLength = 0;
	std::vector<uint8_t> _data;
	std::vector<uint8_t> _receiveData;

	boost::asio::ip::tcp::socket _socket;
};