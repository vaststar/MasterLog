


set(CMAKE_CXX_STANDARD 11)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR} DIR_SRCS)

#build exe
add_executable(testLog ${DIR_SRCS})

target_link_libraries(testLog PRIVATE MasterLog)

add_test (NAME test_log1 COMMAND testLog testConsoleLog)
add_test (NAME test_log2 COMMAND testLog testFileLog)