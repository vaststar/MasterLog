#include "LogExport.h"
#include "LogControl.h"

void WriteLog(const std::string &logTag, MasterLog::LogLevel logLevel, const std::string &filePath, 
                          int lineNumber,const std::string &functionName, const std::string &logMessage)
{
    MasterLog::LogControl::getInstance()->writeLog(logTag,logLevel,filePath,lineNumber,functionName,logMessage);
}

void initLog(int logLevels, int logTypes, const std::string &fullPath)
{
    MasterLog::LogControl::getInstance()->initLogger(logLevels,logTypes,fullPath); 
}