#include <iostream>
#include <thread>
#include "basic.hpp"

using namespace Networking;

BasicUI::BasicUI(Containers::tsQueue<DataTypes::CommunicateData<std::vector<uint8_t>>>* receiveQueue, 
	Containers::tsQueue<DataTypes::CommunicateData<Serialization::ISerializable>>* writeQueue) noexcept
	:_receiveQueue{receiveQueue}, _writeQueue{writeQueue}
{
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

		std::shared_ptr<DataTypes::stringSerialize> tmp = std::make_shared<DataTypes::stringSerialize>(input);
		_writeQueue->push(DataTypes::CommunicateData<Serialization::ISerializable>{ tmp, input.size() });
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