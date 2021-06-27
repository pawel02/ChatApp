#include "basic.hpp"
#include <iostream>

BasicUI::BasicUI(common::tsQueue<common::CommunicateData>* receiveQueue,
	common::tsQueue<common::CommunicateData>* writeQueue) noexcept
	:_receiveQueue{receiveQueue}, _writeQueue{writeQueue}
{}

void BasicUI::start() 
{
	// Endless loop for writing messages to the server
	while (true)
	{
		std::cout << "Write something to the server and hit enter:\n";

		std::string input;
		std::getline(std::cin, input);

		_writeQueue->push(common::CommunicateData{ input.data(), input.size()});
	}

}