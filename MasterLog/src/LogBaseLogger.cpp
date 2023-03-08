#include "LogBaseLogger.h"
#include <functional>
#include "MasterLogConfig.h"

namespace MasterLog{
    LogBaseLogger::LogBaseLogger(int loglevels)
        :m_isInExit(false)
        ,m_loggerLevels(loglevels)
    {

    }

    LogBaseLogger::~LogBaseLogger()
    {
        m_isInExit.store(true);
        m_condition.notify_one();
        if(m_workThread)
        {
            m_workThread->join();
        }
        std::lock_guard<std::mutex> loc(m_witeMutex);
    }

    void LogBaseLogger::startLog()
    {
        std::call_once(start_flag, [&,this]() {
            initialize();
            m_workThread = std::make_shared<std::thread>(std::bind(&LogBaseLogger::doWorkFunction,this));
#if defined(MasterLog_VERSION_MAJOR) && defined(MasterLog_VERSION_MINOR)
            appendLog(LogLevel::LOG_INFO, std::string("Welcome MasterLog, version: ") + MasterLog_VERSION_MAJOR + "." + MasterLog_VERSION_MINOR + "\n");
#endif
        });
    }
    void LogBaseLogger::appendLog( LogLevel loggerLevel, std::string message)
    {
        if(m_isInExit.load() || !(loggerLevel & m_loggerLevels))
        {
            return;
        }
        {
            std::lock_guard<std::mutex> loc(m_dataMutex);
            m_logMessages.emplace(message);
        }
        m_condition.notify_one();
    }
    void LogBaseLogger::doWorkFunction()
    {
        while (!m_isInExit.load())
        {
            std::string currentLog = "";
            {
                std::unique_lock<std::mutex> guard(m_dataMutex);
                m_condition.wait(guard,[this](){return !m_logMessages.empty() || m_isInExit.load();});
                if(m_isInExit.load())
                {
                    return;
                }
                currentLog = m_logMessages.front();
                m_logMessages.pop();
            }
            std::lock_guard<std::mutex> loc(m_witeMutex);
            processMessage(currentLog);
        }
    }
}
