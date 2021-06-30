#include <iostream>
#include "Logger.hpp"

namespace Logger
{
std::mutex ConsoleLogger::_mu;

void ConsoleLogger::log(const LoggerLevel& logLevel, const std::string& message)
{
   switch (logLevel)
   {
       case LoggerLevel::info:
       {
           std::cout << "INFO:: "<< message << "\n";
           break;
       }
       case LoggerLevel::warn:
       {
           std::cout << "WARN:: " << message << "\n";
           break;
       }
       case LoggerLevel::error:
       {
           std::cerr << "ERROR:: " << message << "\n";
           break;
       }
   }
}

}
