#include "LogFileLogger.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <regex>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <cmath> 

#ifdef WIN32
    #include <io.h>
    #include <direct.h>
    #include <Windows.h>
#else
    #include <unistd.h>
    #include <dirent.h>
#endif

#ifdef __APPLE__
    #include <sys/uio.h>
#elif defined(__linux__)
    #include <sys/io.h>
#endif

#ifdef WIN32
static const std::string SPLITSTR = "\\\\";
static const std::string ERROR_SPLITSTR = "/";
#else
static const std::string SPLITSTR = "/";
static const std::string ERROR_SPLITSTR = "\\\\";
#endif

namespace MasterLog{
    LogFileLogger::LogFileLogger(int logLevels, const std::string &logFullPath, unsigned int maxKeepDays, unsigned int maxSignleSize)
        :LogBaseLogger(logLevels)
        ,m_logFilePath(logFullPath)
        ,m_logLevels(logLevels)
        ,m_maxKeepDays(maxKeepDays)
        ,m_maxSingleSize(maxSignleSize)
        ,m_currentSize(0)
    {

    }
    LogFileLogger::~LogFileLogger()
    {
        if(m_currentFile.is_open())
        {
            m_currentFile.flush();
            m_currentFile.close();
        }
    }
    void LogFileLogger::initialize()
    {
        m_logFilePath = std::regex_replace(m_logFilePath,std::regex(ERROR_SPLITSTR.c_str()),SPLITSTR);
        const std::string::size_type last_delimiter = m_logFilePath.rfind(SPLITSTR);
		m_baseFileName = (last_delimiter == std::string::npos)? m_logFilePath : m_logFilePath.substr(last_delimiter+1, m_logFilePath.size()-last_delimiter-1);
        //create dir
        createDir();
    }
    LogAppenderType LogFileLogger::getLoggerType()const
    {
        return LogAppenderType::FILE;
    }
    void LogFileLogger::processMessage( std::string message) 
    {
        if(readyForLog(static_cast<unsigned int>(message.size())) && m_currentFile.is_open())
        {
            m_currentFile<<message;
        }
    }

    void LogFileLogger::createDir()
    {
        std::string dirPath = m_logFilePath;
        if(dirPath.size() < 3)
        {
            return;
        }
        std::regex reg("([a-zA-Z0-9_.-]*)"+SPLITSTR);
        std::smatch sm;
        std::string createPath;
        while(std::regex_search(dirPath,sm,reg))
        {
            if(sm[1].str().empty())
            {
                continue;
            }
            createPath +=  sm[1].str() + SPLITSTR;
            if (::access(createPath.c_str(),0) != 0)
	    	{//说明不存在该目录，创建之
#ifdef WIN32
	    		if (::mkdir(createPath.c_str()) != 0)
	    		{
	    			return;
	    		}
#else
	    		if (::mkdir(createPath.c_str(), 0777) != 0)
	    		{
	    			return;
	    		}
#endif
	    	}		
            dirPath = sm.suffix();
        }
    }

    std::vector<std::string> LogFileLogger::getCurrentFileList()const
    {
        std::vector<std::string> resultVec;
        const std::string::size_type last_delimiter = m_logFilePath.rfind(SPLITSTR);
		const std::string dirname((last_delimiter == std::string::npos)? "." : m_logFilePath.substr(0, last_delimiter));
#ifndef WIN32
		struct dirent **entries;
		int nentries = ::scandir(dirname.c_str(), &entries, 0, ::alphasort);
		if (nentries < 0){
			return resultVec;
        }
		for (int i = 0; i < nentries; i++) {
			struct stat statBuf;
			const std::string fullfilename = dirname + SPLITSTR + entries[i]->d_name;
			int res = ::stat(fullfilename.c_str(), &statBuf);
			if ((res == -1) || (!S_ISREG(statBuf.st_mode))) {
				free(entries[i]);
				continue;
			}
			resultVec.emplace_back(fullfilename);
			free(entries[i]);
		}
		free(entries);
#else
        HANDLE hFind = INVALID_HANDLE_VALUE;
        WIN32_FIND_DATA ffd;
	    const std::string pattern = m_logFilePath + "*";

        hFind = FindFirstFile(pattern.c_str(), &ffd);
        if (hFind != INVALID_HANDLE_VALUE) {
            do {
	    		struct stat statBuf;
	    		const std::string fullfilename = dirname + SPLITSTR + ffd.cFileName;
	    		int res = ::stat(fullfilename.c_str(), &statBuf);
                if (res != -1 && !(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
	    		    resultVec.emplace_back(fullfilename);
                }
            } while (FindNextFile(hFind, &ffd) != 0);
	    	FindClose(hFind);
	    	hFind = INVALID_HANDLE_VALUE;
	    }
#endif
        return resultVec;
    }

    bool LogFileLogger::readyForLog(unsigned int addedSize)
    {
        std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
	    time_t raw_time = std::chrono::system_clock::to_time_t(tp);
	    std::stringstream ss;
	    ss << std::put_time(std::localtime(&raw_time), "%Y-%m-%d");
        std::string nowDate(ss.str());

        if(nowDate != m_currentFileDate)
        {
            if(m_currentFile.is_open())
            {
                m_currentFile.flush();
                m_currentFile.close();
            }
            m_currentFileDate = nowDate;
        }

        //get all files in current dir
        std::vector<std::string> allFiles = getCurrentFileList();
        //delete old file
        removeOldFiles(allFiles);
        //roll over too large file
        doRollOver(allFiles,addedSize);

        //open file
        m_currentSize += addedSize;
        if(!m_currentFile.is_open())
        {
            m_currentFile.open((m_logFilePath+"-"+m_currentFileDate+".log").c_str(), std::ios::app);
            return m_currentFile.is_open();
        }
        return true;
    }

    void LogFileLogger::removeOldFiles(const std::vector<std::string> &allFiles)
    {
        std::for_each(allFiles.begin(),allFiles.end(),[this](const std::string &filePath){
            std::smatch sm;
            if(std::regex_match(filePath,sm,std::regex(".*?"+m_baseFileName+"-(\\d{4}-\\d{2}-\\d{2})\\.log.*?")))
            {
                std::tm tm;
                std::istringstream ss(sm[1].str()+" 0:0:0");
                ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S"); // or just %T in this case
                time_t file_time = std::mktime(&tm);
                std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
	            time_t raw_time = std::chrono::system_clock::to_time_t(tp);
                if(std::abs(difftime(raw_time,file_time)) > static_cast<double>(m_maxKeepDays)*24*3600)
                {
                    std::cout<<"delete too old file:"<<filePath<<std::endl;
                    ::unlink(filePath.c_str());
                }
            }
        });
    }
    void LogFileLogger::doRollOver(const std::vector<std::string> &allFiles,unsigned int addedSize)
    {
        if(!m_currentFile.is_open())
        {
            struct stat statBuf;
            if(0 == ::stat((m_logFilePath+"-"+m_currentFileDate+".log").c_str(),&statBuf))
            {
                m_currentSize = static_cast<unsigned int>(statBuf.st_size);
            }
        }
        if(m_currentSize + addedSize > m_maxSingleSize)
        {
            //do roll over
            if(m_currentFile.is_open())
            {
                m_currentFile.flush();
                m_currentFile.close();
            }
            std::vector<std::string> renameVec;
            std::for_each(allFiles.begin(),allFiles.end(),[this,&renameVec](const std::string &filePath){
                std::smatch sm;
                if(std::regex_match(filePath,sm,std::regex(".*?"+m_baseFileName+"-"+m_currentFileDate+"\\.log.*?")))
                {
                    renameVec.push_back(filePath);
                }
            });
            if(!renameVec.empty())
            {
                std::stable_sort(renameVec.begin(),renameVec.end(),[](const std::string &ls, const std::string &rs){
                    std::smatch sm1,sm2;
                    if(std::regex_match(ls,sm1,std::regex(".*?\\.log$")))
                    {
                        return false;
                    }
                    if(std::regex_match(rs,sm2,std::regex(".*?\\.log$")))
                    {
                        return true;
                    }
                    if(std::regex_match(ls,sm1,std::regex(".*?\\.log\\.(\\d+)$")) && std::regex_match(rs,sm2,std::regex(".*?\\.log\\.(\\d+)$")))
                    {
                        return std::stoi(sm1[1].str()) > std::stoi(sm2[1].str()); 
                    }
                    return true;
                });
                std::for_each(renameVec.begin(),renameVec.end(),[](const std::string &renamePath){
                    std::smatch sm;
                    if(std::regex_match(renamePath,sm,std::regex(".*?\\.log$")))
                    {
                        ::rename(renamePath.c_str(),(renamePath+".1").c_str());
                    }
                    else if(std::regex_match(renamePath,sm,std::regex(".*?\\.log\\.(\\d+)$")))
                    {
                        ::rename(renamePath.c_str(),(std::regex_replace(renamePath,std::regex("(\\d+)$"),std::to_string(std::stoi(sm[1].str())+1))).c_str());
                    }
                });
            }
            m_currentSize = 0;
        }
    }
}