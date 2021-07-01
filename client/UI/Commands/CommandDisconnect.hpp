#pragma once
#include "Command.hpp"

class CommandDisconnect : public Command
{
    template<typename T>
    using tsQueue = Containers::tsQueue<Networking::DataTypes::CommunicateData<T>>;

public:
    CommandDisconnect(tsQueue<Networking::Serialization::ISerializable>* writeQueue) noexcept
        :_writeQueue{writeQueue}
    {
    };

    // This will always return true since any text is valid as this command
    // So make sure to put this last in the switch statement / List of commands
    virtual bool isCommand(const std::string& msg) const override
    {
        return msg == command_prefix + "disconnect";
    };
    virtual void handleCommand(const std::string& msg) override
    {
        std::shared_ptr<Networking::DataTypes::stringSerialize> spMsg = std::make_shared<Networking::DataTypes::stringSerialize>("");
        _writeQueue->push(Networking::DataTypes::CommunicateData<Networking::Serialization::ISerializable>{ spMsg, 0 });
    };

    virtual std::string help() const override
    {
        return command_prefix + "disconnect - Disconnects from the server";
    };
private:
    tsQueue<Networking::Serialization::ISerializable>* _writeQueue;
};