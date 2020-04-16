cmake_minimum_required(VERSION 3.1)

project(logger CXX)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    message(STATUS "build MasterLog with Debug mode")
    set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_LIST_DIR}/install/Debug)
else()
    message(STATUS "build MasterLog with Release mode")
    set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_LIST_DIR}/install/Release)
endif()

# Remove 'lib' prefix for shared libraries on Windows
if (WIN32)
  set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif ()

add_subdirectory(logsrc)

#test log
enable_testing()
add_subdirectory(test)
