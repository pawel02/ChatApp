#pragma once
#include "Command.hpp"

class CommandText : public Command
{
    template<typename T>
	using tsQueue = Containers::tsQueue<Networking::DataTypes::CommunicateData<T>>;

public:
    CommandText(tsQueue<Networking::Serialization::ISerializable>* writeQueue);

    // This will always return true since any text is valid as this command
    // So make sure to put this last in the switch statement / List of commands
    virtual bool isCommand(const std::string& command) const override;
    virtual void handleCommand(const std::string& msg) override;
    virtual std::string help() const override { return  ""; }

private:
    tsQueue<Networking::Serialization::ISerializable>* _writeQueue;
};