#pragma once
#include <CommonLib.hpp>


/*
Simple Command line UI
*/
class BasicUI
{
public:
	BasicUI(common::tsQueue<common::CommunicateData>* receiveQueue, 
		common::tsQueue<common::CommunicateData>* writeQueue) noexcept;

	void start();

private:
	void handleInput();
	void handleReceive();

private:
	common::tsQueue<common::CommunicateData>* _receiveQueue;
	common::tsQueue<common::CommunicateData>* _writeQueue;
};