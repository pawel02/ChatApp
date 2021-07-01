#pragma once
#include <CommonLib.hpp>
#include <string_view>
#include <string>
#include <exception>

/*
Abstract class that is inherited from to handle different string commands
*/
class Command
{

public:
    virtual ~Command() = default;

    // Checks whether the message corresponds to this class
    virtual bool isCommand(const std::string& command) const = 0;

    virtual void handleCommand(const std::string& msg)
    {
        ERROR("Command has no implementation");
        throw new std::exception("Uknown command");
    }

    virtual std::string help() const = 0;

protected:
    std::string command_prefix = "!";
};
