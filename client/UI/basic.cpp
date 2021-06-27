#include <iostream>
#include <thread>
#include "basic.hpp"

BasicUI::BasicUI(common::tsQueue<common::CommunicateData>* receiveQueue,
	common::tsQueue<common::CommunicateData>* writeQueue) noexcept
	:_receiveQueue{receiveQueue}, _writeQueue{writeQueue}
{}

void BasicUI::start() 
{
	std::thread inputThread([&]() {
		handleInput();
		});

	std::thread receiveThread([&]() {
		handleReceive();
		});

	inputThread.join();
	receiveThread.join();
}

void BasicUI::handleInput()
{
	// Endless loop for writing messages to the server
	while (true)
	{
		std::cout << "Write something to the server and hit enter:\n";

		std::string input;
		std::getline(std::cin, input);

		_writeQueue->push(common::CommunicateData{ (uint8_t*)input.data(), input.size() });
	}
}

void BasicUI::handleReceive()
{
	while (true)
	{
		common::CommunicateData data = _receiveQueue->pop();

		// for the moment simply output the message
		std::cout.write((char*)data._data.get(), data._size);
		std::cout << "\n";
	}
}