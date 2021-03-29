project(Google_tests)

enable_testing()
include(GoogleTest)
cmake_minimum_required(VERSION 3.17)
# 'lib' is the folder with Google Test sources
add_subdirectory(lib/googletest-master)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# 'Google_Tests_run' is the target name
# 'test1.cpp tests2.cpp' are source files with tests
add_executable(Google_Tests_run sharedFilesTest.cpp ../SharedFiles.h ../SharedFiles.cpp)
target_link_libraries(Google_Tests_run gtest gtest_main)
