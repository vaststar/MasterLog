#include "LogConsoleLogger.h"
#include <iostream>

namespace LogLogSpace{
    LogConsoleLogger::LogConsoleLogger(int loggerLevels)
        :LogBaseLogger(loggerLevels)
    {

    }
    LogAppenderType LogConsoleLogger::getLoggerType()const
    {
        return LogAppenderType::CONSOLE;
    }
    void LogConsoleLogger::initialize()
    {
        
    }
    void LogConsoleLogger::processMessage(const std::string& message) 
    {
        std::cout<<message<<std::endl;
    }
}
