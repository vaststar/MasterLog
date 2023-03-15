#include "LogBaseLogger.h"

#include <functional>

#include "MasterLogConfig.h"

namespace LogLogSpace{
    LogBaseLogger::LogBaseLogger(int loglevels)
        :m_isInExit(false)
        ,m_loggerLevels(loglevels)
    {

    }

    LogBaseLogger::~LogBaseLogger()
    {
        {
            std::scoped_lock<std::mutex> loc(m_dataMutex);
            m_isInExit = true;
            m_condition.notify_one();
        }
        if(m_workThread)
        {
            m_workThread->join();
        }
        std::scoped_lock<std::mutex> loc(m_witeMutex);
    }

    void LogBaseLogger::startLog()
    {
        std::call_once(start_flag, [&,this]() {
            initialize();
            m_workThread = std::make_unique<std::thread>(std::bind(&LogBaseLogger::doWorkFunction,this));
#if defined(MasterLog_VERSION_MAJOR) && defined(MasterLog_VERSION_MINOR)
            appendLog(LogLevel::LOG_INFO, std::string("Welcome MasterLog, version: ") + MasterLog_VERSION_MAJOR + "." + MasterLog_VERSION_MINOR + "\n");
#endif
        });
    }
    void LogBaseLogger::appendLog( LogLevel loggerLevel, const std::string& message)
    {
        std::scoped_lock<std::mutex> loc(m_dataMutex);
        if(m_isInExit || !(loggerLevel & m_loggerLevels))
        {
            return;
        }
        m_logMessages.push(message);
        m_condition.notify_one();
    }

    void LogBaseLogger::doWorkFunction()
    {
        while (true)
        {
            std::string currentLog = "";
            {
                std::unique_lock<std::mutex> guard(m_dataMutex);
                m_condition.wait(guard,[this](){return !m_logMessages.empty() || m_isInExit;});
                if(m_isInExit)
                {
                    return;
                }
                currentLog = m_logMessages.front();
                m_logMessages.pop();
            }
            std::scoped_lock<std::mutex> loc(m_witeMutex);
            processMessage(currentLog);
        }
    }
}
