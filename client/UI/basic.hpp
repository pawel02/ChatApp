#pragma once
#include <CommonLib.hpp>


/*
Simple Command line UI
*/
class BasicUI
{
public:
	BasicUI(Containers::tsQueue<Networking::DataTypes::CommunicateData<std::vector<uint8_t>>>* receiveQueue,
		Containers::tsQueue<Networking::DataTypes::CommunicateData<Networking::Serialization::ISerializable>>* writeQueue) noexcept;

	void start();

private:
	void handleInput();
	void handleReceive();

private:
	Containers::tsQueue<Networking::DataTypes::CommunicateData<std::vector<uint8_t>>>* _receiveQueue;
	Containers::tsQueue<Networking::DataTypes::CommunicateData<Networking::Serialization::ISerializable>>* _writeQueue;
};