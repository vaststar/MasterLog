#include "LogExport.h"
#include <iostream>

#include <list>
#include <thread>
#include <chrono>

#define TEST_LOG_DEBUG(message) LOG_DEBUG("TEST_LOG",message)

void testConsoleLog()
{
	auto configure = std::make_shared<LogLogSpace::LoggerConsoleConfigure>(MasterLogUtil::ALL_LOG_LEVEL);
	MasterLogUtil::InitLogger({configure});
	TEST_LOG_DEBUG("startTest");
	std::list<std::thread> aaa;
	for (int i = 0;i < 3000 ;++i)
	{
		//TEST_LOG_DEBUG("test: "<<i);
		aaa.emplace_back([i]() {
			TEST_LOG_DEBUG(i);
		});
	}
	for (auto it = aaa.begin();it != aaa.end();++it)
	{
		(*it).join();
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void testFileLog()
{
	std::string dirPath = "./testMkdir/teestDD";
	std::string baseFileName = "testD";
	auto configure = std::make_shared<LogLogSpace::LoggerFileConfigure>(MasterLogUtil::ALL_LOG_LEVEL, dirPath, baseFileName, 180, 20*1024*1024);
	MasterLogUtil::InitLogger({configure});
	std::list<std::thread> aaa;
	for (int i = 0;i < 2000 ;++i)
	{
		//TEST_LOG_DEBUG("test: "<<i);
		aaa.emplace_back([i]() {
			TEST_LOG_DEBUG(i);
		});
	}
	for (auto it = aaa.begin();it != aaa.end();++it)
	{
		(*it).join();
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

}
int main(int argc,char *argv[])
{
	if(argc >= 2)
	{
		if(std::string(argv[1]) == "testConsoleLog")
		{
			testConsoleLog();
		}
		else if(std::string(argv[1]) == "testFileLog")
		{
			testFileLog();
		}
		else if(std::string(argv[1]) == "testConsoleFileLog")
		{
			testConsoleLog();
			testFileLog();
		}
	}
	return 0;
}