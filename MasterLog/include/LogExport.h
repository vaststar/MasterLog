#ifndef __LogExport_h__
#define __LogExport_h__

#include <memory>
#include <string>
#include <ostream>
#include <sstream>
#include "LogDefine.h"

#ifdef MASTERLOG_DLL
#ifdef MASTERLOG_LIB
#define LOG_LIB_API _declspec(dllexport)
#else
#define LOG_LIB_API _declspec(dllimport)
#endif
#else 
#define LOG_LIB_API 
#endif

namespace MasterLogUtil{
static const int ALL_LOG_LEVEL = LogLogSpace::LogLevel::LOG_DEBUG|LogLogSpace::LogLevel::LOG_INFO|LogLogSpace::LogLevel::LOG_WARN|LogLogSpace::LogLevel::LOG_ERROR|LogLogSpace::LogLevel::LOG_FATAL;
static const int EXCLUDE_DEBUG_LOG_LEVEL = LogLogSpace::LogLevel::LOG_INFO|LogLogSpace::LogLevel::LOG_WARN|LogLogSpace::LogLevel::LOG_ERROR|LogLogSpace::LogLevel::LOG_FATAL;

LOG_LIB_API void WriteLog(const std::string& logTag, LogLogSpace::LogLevel logLevel, const std::string& filePath, 
                          int lineNumber, const std::string& functionName, const std::string& logMessage);
LOG_LIB_API void initAllLogs(const std::string& fullPath, int logLevels = ALL_LOG_LEVEL, unsigned int maxKeepDays = 365, unsigned int maxSingleFileSize = 20*1024*1024);
LOG_LIB_API void initConsoleLog(int logLevels = ALL_LOG_LEVEL);
LOG_LIB_API void initFileLog(const std::string& fullPath, int logLevels = ALL_LOG_LEVEL, unsigned int maxKeepDays = 365, unsigned int maxSingleFileSize = 20*1024*1024);
}

#define LOG_DEBUG(logTag,logMessage) {std::ostringstream _oss; _oss << logMessage;MasterLogUtil::WriteLog(logTag,LogLogSpace::LogLevel::LOG_DEBUG,__FILE__,__LINE__,__FUNCTION__,_oss.str());}
#define LOG_INFO(logTag,logMessage) {std::ostringstream _oss; _oss << logMessage;MasterLogUtil::WriteLog(logTag,LogLogSpace::LogLevel::LOG_INFO,__FILE__,__LINE__,__FUNCTION__,_oss.str());}
#define LOG_WARN(logTag,logMessage) {std::ostringstream _oss; _oss << logMessage;MasterLogUtil::WriteLog(logTag,LogLogSpace::LogLevel::LOG_WARN,__FILE__,__LINE__,__FUNCTION__,_oss.str());}
#define LOG_ERROR(logTag,logMessage) {std::ostringstream _oss; _oss << logMessage;MasterLogUtil::WriteLog(logTag,LogLogSpace::LogLevel::LOG_ERROR,__FILE__,__LINE__,__FUNCTION__,_oss.str());}
#define LOG_FATAL(logTag,logMessage) {std::ostringstream _oss; _oss << logMessage;MasterLogUtil::WriteLog(logTag,LogLogSpace::LogLevel::LOG_FATAL,__FILE__,__LINE__,__FUNCTION__,_oss.str());}
#endif//__LogExport_h__