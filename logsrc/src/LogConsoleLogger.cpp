#include "LogConsoleLogger.h"
#include <iostream>

namespace MasterLog{
    LogConsoleLogger::LogConsoleLogger()
    {

    }
    LogAppenderType LogConsoleLogger::getLoggerType()const
    {
        return LogAppenderType::CONSOLE;
    }
    void LogConsoleLogger::initialize()
    {
        
    }
    void LogConsoleLogger::processMessage( std::string message) 
    {
        std::cout<<std::move(message)<<std::endl;
    }
}