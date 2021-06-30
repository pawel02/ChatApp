#pragma once
#include <cstdint>
#include <string>
#include <mutex>

namespace Logger
{
    enum class LoggerLevel : uint8_t
    {
        info, warn, error
    };

    // Thread safe singleton logger
    class ConsoleLogger
    {
    public:
        static ConsoleLogger& Instance()
        {
            std::lock_guard<std::mutex> lock{ _mu };
            static ConsoleLogger logger{};

            return logger;
        }

        void log(const LoggerLevel& logLevel, const std::string& message);
    private:
        ConsoleLogger() = default;

        static std::mutex _mu;
    };
}

#ifdef _DEBUG
    #define INFO(msg) Logger::ConsoleLogger::Instance().log(Logger::LoggerLevel::info, msg)
    #define WARN(msg) Logger::ConsoleLogger::Instance().log(Logger::LoggerLevel::warn, msg)
    #define ERROR(msg) Logger::ConsoleLogger::Instance().log(Logger::LoggerLevel::error, msg)
#else
#define INFO(msg)
#define WARN(msg)
#define ERROR(msg)
#endif