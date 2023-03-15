#ifndef __LogFileLogger_h__
#define __LogFileLogger_h__
#include <string>
#include <fstream>
#include <vector>
#include "LogDefine.h"
#include "LogBaseLogger.h"
namespace LogLogSpace{
    class LogFileLogger final: public LogBaseLogger
    {
    public:
        LogFileLogger(int logLevels, const std::string& logFullPath, unsigned int maxKeepDays, unsigned int maxSignleSize);
        ~LogFileLogger();
        LogAppenderType getLoggerType()const override;
    protected:
        void initialize() override;
        void processMessage(const std::string& message) override;
    private:
        void createDir();
        std::vector<std::string> getCurrentFileList()const;
        bool readyForLog(unsigned int addedSize);
        void removeOldFiles(const std::vector<std::string>& allFiles);
        void doRollOver(const std::vector<std::string>& allFiles,unsigned int addedSize);
    private:
        std::string m_logFilePath;
        int m_logLevels;
        unsigned int m_maxKeepDays;
        unsigned int m_maxSingleSize;
        std::string m_baseFileName;
	    std::ofstream m_currentFile;
        std::string m_currentFileDate;
        unsigned int m_currentSize;
    };
}
#endif//__LogFileLogger_h__