#pragma once
#include <vector>
#include <memory>
#include <CommonLib.hpp>
#include "./Commands/Command.hpp"

/*
Simple Command line UI
*/
class BasicUI
{
	template<typename T>
	using tsQueue = Containers::tsQueue<Networking::DataTypes::CommunicateData<T>>;

public:
	BasicUI(tsQueue<std::vector<uint8_t>>* receiveQueue,
		tsQueue<Networking::Serialization::ISerializable>* writeQueue) noexcept;

	void start();

private:
	void handleInput();
	void handleReceive();

private:
	tsQueue<std::vector<uint8_t>>* _receiveQueue;
	tsQueue<Networking::Serialization::ISerializable>* _writeQueue;

	std::vector<std::unique_ptr<Command>> _commands;
};