#ifndef __LogControl_h__
#define __LogControl_h__

#include <memory>
#include <atomic>
#include <string>
#include <list>
#include <mutex>

#include "LogDefine.h"
#include "LogBaseLogger.h"

namespace MasterLog{
    class LogControl
    {
    public:
        static std::shared_ptr<LogControl> getInstance();

        void initConsoleLogger(int logLevels);
        void initFileLogger( int logLevels, const std::string &logFullPath, unsigned int maxKeepDays, unsigned int maxSingleFileSize);
        void writeLog(const std::string &logTag, LogLevel logLevel, const std::string &filePath, 
                      int lineNumber,const std::string &functionName, const std::string &logMessage);
    private:
        std::mutex m_loggerMutex;
        std::list<std::shared_ptr<LogBaseLogger>> m_currentLogger;
    private:
        void createLoggers();
        std::string formatMessage(const std::string &logTag, LogLevel logLevel, const std::string &filePath, 
                                  int lineNumber,const std::string &functionName, const std::string &logMessage)const;
    private:
        LogControl();
        static std::shared_ptr<LogControl> _instance;
    public:
        LogControl(const LogControl &rhs) = delete;
        LogControl& operator=(const LogControl &rhs) = delete;
        LogControl(LogControl &&rhs) = delete;
        LogControl& operator=(LogControl &&rhs) = delete;
        ~LogControl();
    };
}
#endif// __LogControl_h__