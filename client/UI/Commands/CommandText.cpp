#include <memory>
#include "CommandText.hpp"

using namespace Networking;

CommandText::CommandText(tsQueue<Networking::Serialization::ISerializable>* writeQueue)
    :_writeQueue{writeQueue}
{}

bool CommandText::isCommand(const std::string& command) const
{
    return true;
}

void CommandText::handleCommand(const std::string& msg)
{
    std::shared_ptr<DataTypes::stringSerialize> spMsg = std::make_shared<DataTypes::stringSerialize>(msg);
    _writeQueue->push(DataTypes::CommunicateData<Serialization::ISerializable>{ spMsg, msg.size() });
}