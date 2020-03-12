#include "LogFileLogger.h"

//#include <experimental/filesystem>

namespace MasterLog{
    LogFileLogger::LogFileLogger(int logLevels, const std::string &logFullPath, int maxKeepDays)
        :LogBaseLogger(logLevels)
        ,m_logFilePath(logFullPath)
        ,m_logLevels(logLevels)
        ,m_maxKeepDays(maxKeepDays)
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