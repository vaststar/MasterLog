#include "LogExport.h"
#include "LogControl.h"

namespace MasterLogUtil{
void WriteLog(const std::string& logTag, LogLogSpace::LogLevel logLevel, const std::string& filePath, 
                          int lineNumber,const std::string& functionName, const std::string& logMessage)
{
    LogLogSpace::LogControl::getInstance()->writeLog(logTag,logLevel,filePath,lineNumber,functionName,logMessage);
}

void initAllLogs(const std::string& fullPath, int logLevels , int maxKeepDays)
{
    initConsoleLog(logLevels);
    initFileLog(fullPath, logLevels, maxKeepDays);
}

void initConsoleLog(int logLevels )
{
    LogLogSpace::LogControl::getInstance()->initConsoleLogger(logLevels); 
}

void initFileLog(const std::string& fullPath, int logLevels, unsigned int maxKeepDays, unsigned int maxSingleFileSize)
{
    LogLogSpace::LogControl::getInstance()->initFileLogger(logLevels,fullPath,maxKeepDays,maxSingleFileSize); 
}
}
