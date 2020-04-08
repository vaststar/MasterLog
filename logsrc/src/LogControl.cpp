
#include <mutex>
#include <thread>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "LogControl.h"
#include "LogConsoleLogger.h"
#include "LogFileLogger.h"

namespace MasterLog{
std::shared_ptr<LogControl> LogControl::_instance = nullptr;
std::shared_ptr<LogControl> LogControl::getInstance()
{
    static std::once_flag s_flag;
    std::call_once(s_flag, [&]() {
        _instance.reset(new LogControl());
    });
    return _instance;
}

LogControl::LogControl()
{

}

LogControl::~LogControl()
{
    std::lock_guard<std::mutex> lo(m_loggerMutex);
    m_currentLogger.clear();
}

void LogControl::initConsoleLogger(int logLevels)
{
    static std::once_flag init_console_flag;
    std::call_once(init_console_flag, [logLevels,this]() {
        std::lock_guard<std::mutex> lo(m_loggerMutex);
        m_currentLogger.emplace_back(std::make_shared<LogConsoleLogger>(logLevels));
        m_currentLogger.back()->startLog();
    });
}

void LogControl::initFileLogger( int logLevels, const std::string &logFullPath, unsigned int maxKeepDays, unsigned int maxSingleFileSize)
{
    static std::once_flag init_file_flag;
    std::call_once(init_file_flag, [logLevels,logFullPath,maxKeepDays,maxSingleFileSize,this]() {
        std::lock_guard<std::mutex> lo(m_loggerMutex);
        m_currentLogger.emplace_back(std::make_shared<LogFileLogger>(logLevels,logFullPath,maxKeepDays,maxSingleFileSize));
        m_currentLogger.back()->startLog();
    });
}

void LogControl::writeLog(const std::string &logTag, LogLevel logLevel, const std::string &filePath, 
              int lineNumber,const std::string &functionName, const std::string &logMessage)
{
    std::string messageLog = formatMessage(logTag,logLevel,filePath,lineNumber,functionName,logMessage);
    std::lock_guard<std::mutex> lo(m_loggerMutex);
    std::for_each(m_currentLogger.begin(),m_currentLogger.end(),[logLevel,&messageLog,this](std::shared_ptr<LogBaseLogger> logger){
        logger->appendLog(logLevel,messageLog);
    });
}
std::string LogControl::formatMessage(const std::string &logTag, LogLevel logLevel, const std::string &filePath, 
                          int lineNumber,const std::string &functionName, const std::string &logMessage)const
{
    //logLevel
    std::string levelMessage = "Debug";
    switch (logLevel)
    {
    case LogLevel::LOG_DEBUG:
        levelMessage = "DEBUG";
        break;
    case LogLevel::LOG_INFO:
        levelMessage = "INFO ";
        break;
    case LogLevel::LOG_WARN:
        levelMessage = "WARN ";
        break;
    case LogLevel::LOG_ERROR:
        levelMessage = "ERROR";
        break;
    case LogLevel::LOG_FATAL:
        levelMessage = "FATAL";
        break;
    default:
        break;
    }
    //time
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
	time_t raw_time = std::chrono::system_clock::to_time_t(tp);
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
	std::string milliseconds_str = std::to_string(ms.count() % 1000);
	if (milliseconds_str.length() < 3) {
		milliseconds_str = std::string(3 - milliseconds_str.length(), '0') + milliseconds_str;
	}
	std::stringstream ss;
	ss << std::put_time(std::localtime(&raw_time), "%Y-%m-%d %H:%M:%S,")<<milliseconds_str;
    std::string timeString = ss.str();
    //filepath+linenumber
    std::string fileString = filePath + "(" + std::to_string(lineNumber) + ")";
    if(fileString.length() > 40)
    {
        fileString = fileString.substr(fileString.length()-40,40);
    }
    else if(fileString.length() < 40)
    {
        fileString = std::string(40 - fileString.length(),' ') + fileString;
    }
    
    std::stringstream result;
    result << timeString << " " << levelMessage << " [" << std::this_thread::get_id() << "] ["<< fileString << "] ["
           << logTag << "] [" << functionName << "] - " << logMessage <<"\n";
    return result.str();
}
}
