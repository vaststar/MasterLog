#ifndef __LogFileLogger_h__
#define __LogFileLogger_h__
#include <string>
#include "LogDefine.h"
#include "LogBaseLogger.h"
namespace MasterLog{
    class LogFileLogger: public LogBaseLogger
    {
    public:
        explicit LogFileLogger(const std::string &logFilePath);
        LogFileLogger();
        ~LogFileLogger();
        LogAppenderType getLoggerType()const override;
    protected:
        void initialize() override;
        void processMessage(std::string message) override;
    private:
        std::string m_logFilePath;
    };
}
#endif//__LogFileLogger_h__