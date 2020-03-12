#ifndef __LogConsoleLogger_h__
#define __LogConsoleLogger_h__
#include <string>
#include "LogDefine.h"
#include "LogBaseLogger.h"
namespace MasterLog{
    class LogConsoleLogger: public LogBaseLogger
    {
    public:
        explicit LogConsoleLogger(int loggerLevels);
        ~LogConsoleLogger()=default;
        LogAppenderType getLoggerType()const override;
    protected:
        void initialize() override;
        void processMessage(std::string message) override;
    };
}
#endif//__LogConsoleLogger_h__