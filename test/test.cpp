#include "LogExport.h"
#include <iostream>

#include <list>
#include <thread>
#include <chrono>

#define TEST_LOG_DEBUG(message) LOG_DEBUG("TEST_LOG",message)

void testConsoleLog()
{
	initConsoleLog();
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
	initFileLog(".\\testMkdir\\tedir\\test",ALL_LOG_LEVEL,365,2*1024*1024);
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