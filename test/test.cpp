#include "LogExport.h"
#include <iostream>

#include <list>
#include <thread>
#include <chrono>

#define TEST_LOG_DEBUG(message) LOG_DEBUG("TEST_LOG",message)

void testConsoleLog()
{
	initLog(ALL_LOG_LEVEL,(int)MasterLog::LogAppenderType::CONSOLE);
	std::list<std::thread> aaa;
	for (int i = 0;i < 200 ;++i)
	{
		TEST_LOG_DEBUG("test: "<<i);
		aaa.emplace_back([i]() {
			TEST_LOG_DEBUG(i);
		});

	}
	for (auto it = aaa.begin();it != aaa.end();++it)
	{
		(*it).join();
	}
	std::this_thread::sleep_for(std::chrono::seconds(1));
}
void testFileLog()
{
	initLog(ALL_LOG_LEVEL,(int)MasterLog::LogAppenderType::FILE,"/");
	std::list<std::thread> aaa;
	for (int i = 0;i < 200 ;++i)
	{
		TEST_LOG_DEBUG("test: "<<i);
		aaa.emplace_back([i]() {
			TEST_LOG_DEBUG(i);
		});

	}
	for (auto it = aaa.begin();it != aaa.end();++it)
	{
		(*it).join();
	}
	std::this_thread::sleep_for(std::chrono::seconds(1));

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
	}
	return 0;
}