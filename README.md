## Thread-Safe log using C++

## Usage

* `#include "LogExport.h"`
* define your own macro with log-tag like `#define TEST_LOG_DEBUG(message) LOG_DEBUG("TEST_LOG_TAG",message)`
* init log type , console: `initConsoleLog();`; filelog: `initFileLog(".\\hhh\\tedir\\test",ALL_LOG_LEVEL,365,2*1024*1024);`
* use log macro in your code, eg`TEST_LOG_DEBUG("testlog"<<111);`

## Requirement
* >= c++11
* >= cmake3.1

### install

* make and cd in `build` dir
* run commond: `cmake .. && make && make install`
