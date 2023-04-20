# Thread-Safe log using C++
A very nice log in c++20 written by Thomas Zhu.

# Usage
* firstly, link library in your project-set
* write some common code in your codings

## Using in project
* install first
* set dir in your CMakeList,eg `set(MasterLog_DIR $logInstalledDir$/MasterLog/install/Release/cmake) `
* use find package , `find_package(MasterLog REQUIRED)`
* include headers, `include_directories(${MasterLog_INCLUDE_DIRS})`
* link target to you library, `target_link_libraries(testProgram MasterLogExport::MasterLog)`

## Using in code
* `#include "LogExport.h"`
* define your own macro with log-tag like `#define TEST_LOG_DEBUG(message) LOG_DEBUG("TEST_LOG_TAG",message)`
* init logger with configure
```c++
//define your configure, print all level log
auto configure = std::make_shared<LogLogSpace::LoggerConsoleConfigure>(MasterLogUtil::ALL_LOG_LEVEL);
MasterLogUtil::InitLogger({configure});
```
```c++
//define your configure, eg. store at ./testMkdir/teestDD/testFile-2022-03-04.log
std::string dirPath = "./testMkdir/teestDD";//dirpath
std::string baseFileName = "testFile";//file name
unsigned int maxKeepDays = 180; //180day
unsigned int maxSingleFileSize = 20*1024*1024; //20M
auto configure = std::make_shared<LogLogSpace::LoggerFileConfigure>(MasterLogUtil::ALL_LOG_LEVEL, dirPath,baseFileName, maxKeepDays, maxSingleFileSize);
MasterLogUtil::InitLogger({configure});
```
* use log macro in your code, eg `TEST_LOG_DEBUG("testlog"<<111);`

# Requirement
* >= c++20
* >= cmake3.11

# Install
* make and cd in `build` dir
* run commond: `cmake .. && make && make install`

# Test
* make and cd in `build` dir
* run commond: `cmake .. && make && ctest -V`
