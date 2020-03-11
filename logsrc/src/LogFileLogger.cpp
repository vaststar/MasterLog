#include "LogFileLogger.h"

//#include <experimental/filesystem>

namespace MasterLog{
    LogFileLogger::LogFileLogger(const std::string &logFilePath)
        :m_logFilePath(logFilePath)
    {

    }
    LogFileLogger::~LogFileLogger()
    {

    }
    void LogFileLogger::initialize()
    {
        //deal with filepath
    }
    LogAppenderType LogFileLogger::getLoggerType()const
    {
        return LogAppenderType::FILE;
    }
    void LogFileLogger::processMessage( std::string message) 
    {

    }
}