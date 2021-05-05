project(Google_tests)

enable_testing()
include(GoogleTest)
cmake_minimum_required(VERSION 3.16.3)
# 'lib' is the folder with Google Test sources
add_subdirectory(lib/googletest-master)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# 'Google_Tests_run' is the target name
# 'test1.cpp tests2.cpp' are sour ce files with tests
add_executable(Google_Tests_run SharedFilesTest.cpp JobTest.cpp ../Job.h ../Job.cpp ../SharedFiles.h ../SharedFiles.cpp ../pinger.cpp ../pinger.h )
target_link_libraries(Google_Tests_run gtest gtest_main)
