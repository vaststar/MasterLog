# Thread-Safe log using C++
A very nice log in c++11 written by Thomas Zhu.

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
* init log type , console: `MasterLogUtil::InitLogger({std::make_shared<LogLogSpace::LoggerConsoleConfigure>(MasterLogUtil::ALL_LOG_LEVEL)});`; filelog: `MasterLogUtil::InitLogger(std::make_shared<LogLogSpace::LoggerFileConfigure>(MasterLogUtil::ALL_LOG_LEVEL, dirPath, baseFileName, 180, 20*1024*1024)})`
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
