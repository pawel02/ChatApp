#pragma once
#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <vector>
#include <any>
#include <CommonLib.hpp>

/*
TCP full-duplex communication client
The client will automatically close if no data is received from the server for 60 seconds
*/
class Client : public std::enable_shared_from_this<Client>
{
	using tcp = boost::asio::ip::tcp;
public:
	Client(boost::asio::io_context* ctx, const std::string& host
		, const std::string& port ) noexcept;

	void connect();

	// So that the UI can get a reference and then add to these queues
	common::tsQueue<common::CommunicateData<Utils::ISerializable>>* writeQueue() noexcept { return &_writeQueue; }
	common::tsQueue<common::CommunicateData<std::vector<uint8_t>>>* receiveQueue() noexcept { return &_receiveQueue; }

private:
	// Infinite loop of reading from the socket and putting the messages on the sharedQueue
	void do_read();

	// Inifinite loop of writing to the client the latest message sent from the UI
	void do_write();

	// Terminates all the actors and shutdowns the connection
	void stop();
	void check_deadline();

private:
	std::string _host;
	std::string _port;

	tcp::socket _socket;
	tcp::resolver _resolver;

	bool _stopped = false;

	const int expiryTime = 60;
	boost::asio::steady_timer _deadline;

	enum {maxLength = 1024};

	uint64_t _messageSize = 0;
	uint64_t receivedLength = 0;
	std::vector<uint8_t> _data;
	std::vector<uint8_t> _receiveData;

	// Queues for communicating between the UI and this thread
	common::tsQueue<common::CommunicateData<Utils::ISerializable>> _writeQueue;
	common::tsQueue<common::CommunicateData<std::vector<uint8_t>>> _receiveQueue;

};