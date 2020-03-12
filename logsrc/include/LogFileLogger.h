#ifndef __LogFileLogger_h__
#define __LogFileLogger_h__
#include <string>
#include "LogDefine.h"
#include "LogBaseLogger.h"
namespace MasterLog{
    class LogFileLogger: public LogBaseLogger
    {
    public:
        explicit LogFileLogger(int logLevels, const std::string &logFullPath, int maxKeepDays);
        ~LogFileLogger();
        LogAppenderType getLoggerType()const override;
    protected:
        void initialize() override;
        void processMessage(std::string message) override;
    private:
    
    private:
        std::string m_logFilePath;
        int m_logLevels;
        int m_maxKeepDays;
    };
}
#endif//__LogFileLogger_h__