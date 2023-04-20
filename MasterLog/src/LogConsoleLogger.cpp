#include "LogConsoleLogger.h"
#include <iostream>

namespace LogLogSpace{
    LogConsoleLogger::LogConsoleLogger(int loggerLevels)
        :LogBaseLogger(loggerLevels)
    {

    }
    
    void LogConsoleLogger::initialize()
    {
        
    }
    
    void LogConsoleLogger::processMessage(const std::string& message) 
    {
        std::cout<<message<<std::endl;
    }
}
