#include "LogExport.h"
#include "LogControl.h"

namespace MasterLogUtil{
void WriteLog(const std::string& logTag, LogLogSpace::LogLevel logLevel, const std::string& filePath, 
                          int lineNumber,const std::string& functionName, const std::string& logMessage)
{
    LogLogSpace::LogControl::getInstance()->writeLog(logTag,static_cast<int>(logLevel),filePath,lineNumber,functionName,logMessage);
}

void InitLogger(const std::vector<std::shared_ptr<LogLogSpace::LoggerBaseConfigure>>& configure)
{
    std::for_each(configure.cbegin(), configure.cend(), [](const std::shared_ptr<LogLogSpace::LoggerBaseConfigure>& config){
        if (auto consoleConfig = std::dynamic_pointer_cast<LogLogSpace::LoggerConsoleConfigure>(config))
        {
            LogLogSpace::LogControl::getInstance()->initConsoleLogger(consoleConfig->loggerLevels); 
        }
        else if (auto fileConfig = std::dynamic_pointer_cast<LogLogSpace::LoggerFileConfigure>(config))
        {
            LogLogSpace::LogControl::getInstance()->initFileLogger(fileConfig->loggerLevels, fileConfig->loggerDirPath, fileConfig->loggerBaseName, fileConfig->maxKeepDays, fileConfig->maxSingleFileSize); 
        }
    });
}
}
