#ifndef __LogBaseLogger_h__
#define __LogBaseLogger_h__

#include <atomic>
#include <string>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <thread>
#include <condition_variable>
#include "LogDefine.h"

namespace MasterLog{
    class LogBaseLogger
    {
    public:
        explicit LogBaseLogger(int loglevels);
        virtual ~LogBaseLogger();
        virtual LogAppenderType getLoggerType()const = 0;
        void appendLog(LogLevel loggerLevel, std::string message);
        void startLog();
    protected:
        virtual void initialize() = 0;
        virtual void processMessage(std::string message) = 0;
    private:
        void doWorkFunction();
    private:
        int m_loggerLevels;
        std::condition_variable m_condition;
        std::mutex m_dataMutex;
        std::atomic<bool> m_isInExit;
        std::queue<std::string> m_logMessages;
        std::shared_ptr<std::thread> m_workThread;
        std::mutex m_witeMutex;
    };
}
#endif//__LogBaseLogger_h__