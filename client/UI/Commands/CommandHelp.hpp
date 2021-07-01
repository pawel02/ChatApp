#pragma once
#include "Command.hpp"

class CommandHelp : public Command
{
public:
    CommandHelp(std::vector<std::unique_ptr<Command>>* commands) noexcept
        :_commands{commands}
    {};

    // This will always return true since any text is valid as this command
    // So make sure to put this last in the switch statement / List of commands
    virtual bool isCommand(const std::string& command) const override
    {
        return command == command_prefix + "help";
    };
    virtual void handleCommand(const std::string& msg) override
    {
        // Output a message containing all the commands
        std::string help;
        help.reserve(1000);
        help.append("Commands are not case sensitive\nAvailable commands:\n\n");
        for (auto&& command : *_commands)
        {
            // Make sure that you don't call this recursively
            if (command.get() != this) 
            {
                help.append(command->help());
                help.append("\n");
            }
        }
        std::cout << help;
    };

    virtual std::string help() const override { return ""; };
private:
    std::vector<std::unique_ptr<Command>>* _commands;
};