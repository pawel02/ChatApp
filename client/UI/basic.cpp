#include <iostream>
#include <algorithm>
#include <thread>
#include "basic.hpp"
#include "./Commands/CommandHelp.hpp"
#include "./Commands/CommandDisconnect.hpp"
#include "./Commands/CommandText.hpp"

using namespace Networking;

BasicUI::BasicUI(Containers::tsQueue<DataTypes::CommunicateData<std::vector<uint8_t>>>* receiveQueue, 
	Containers::tsQueue<DataTypes::CommunicateData<Serialization::ISerializable>>* writeQueue) noexcept
	:_receiveQueue{receiveQueue}, _writeQueue{writeQueue}
{
	// Add all the commands that you want to handle here
	_commands.emplace_back(std::make_unique<CommandHelp>(CommandHelp{ &_commands  }));
	_commands.emplace_back(std::make_unique<CommandDisconnect>(CommandDisconnect{ _writeQueue }));

	// Needs to go at the end
	_commands.emplace_back(std::make_unique<CommandText>(CommandText{ _writeQueue }));
}

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

		std::string inputLower = input;
		std::for_each(inputLower.begin(), inputLower.end(), [](char& c) {
			c = ::tolower(c);
		});

		// Find the correct command in the vector and use it
		bool isHandled = false;
		for (auto&& command : _commands)
		{
			if (command->isCommand(input))
			{
				command->handleCommand(input);
				isHandled = true;
				break;
			}
		}
		if (!isHandled)
		{
			ERROR("Did not handle the command");
		}
	}
}

void BasicUI::handleReceive()
{
	while (true)
	{
		DataTypes::CommunicateData<std::vector<uint8_t>> data = _receiveQueue->pop();

		// figure out what the data format is based on the header use the factory pattern HERE
		// Also make sure that you have the correct number of bytes
		short dataType = *(data._data->begin() + 1);
		if (dataType == 0)
		{
			// This is plain text (Create a type for this)
			char* text = (char*)data._data->data() + Header::headerSize;
			size_t dataSize = data._size;

			std::cout.write(text, dataSize);
			std::cout << "\n";
		}

		// for the moment simply output the message
		/*std::cout << "\n\nMessage from server:\n";
		std::cout.write((char*)data._data->data() + common::headerSize, data._size);
		std::cout << "\n";*/
	}
}