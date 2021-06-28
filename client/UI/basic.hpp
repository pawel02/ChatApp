#pragma once
#include <CommonLib.hpp>


/*
Simple Command line UI
*/
class BasicUI
{
public:
	BasicUI(common::tsQueue<common::CommunicateData<std::vector<uint8_t>>>* receiveQueue,
		common::tsQueue<common::CommunicateData<Utils::ISerializable>>* writeQueue) noexcept;

	void start();

private:
	void handleInput();
	void handleReceive();

private:
	common::tsQueue<common::CommunicateData<std::vector<uint8_t>>>* _receiveQueue;
	common::tsQueue<common::CommunicateData<Utils::ISerializable>>* _writeQueue;
};