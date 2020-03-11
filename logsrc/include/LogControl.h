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
    class LOG_LIB_API LogControl
    {
    public:
        static std::shared_ptr<LogControl> getInstance();

        void initLogger(int logLevel, int logTypes, const std::string &logFullPath);
        void writeLog(const std::string &logTag, LogLevel logLevel, const std::string &filePath, 
                      int lineNumber,const std::string &functionName, const std::string &logMessage);
    public:
        const std::string &getRootPath()const;
        int getLogLevels()const;
        int getLogTypes()const;
    private:
        std::atomic<int> m_logLevels;
        std::atomic<int> m_logTypes;
        std::string m_logFullPath;
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