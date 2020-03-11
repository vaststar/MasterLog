#include "LogBaseLogger.h"

namespace MasterLog{
    LogBaseLogger::LogBaseLogger()
        :m_isInExit(false)
    {

    }

    LogBaseLogger::~LogBaseLogger()
    {
        m_isInExit.store(true);
        m_condition.notify_one();
        m_workThread->join();
    }

    void LogBaseLogger::startLog()
    {
        static std::once_flag start_flag;
        std::call_once(start_flag, [&,this]() {
            m_workThread = std::make_shared<std::thread>(std::bind(&LogBaseLogger::doWorkFunction,this));
        });
    }
    void LogBaseLogger::appendLog( std::string message)
    {
        if(m_isInExit.load())
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