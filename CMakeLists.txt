cmake_minimum_required(VERSION 3.17)
project(HTStorage)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(Google_tests)
add_executable(HTStorage main.cpp pinger.h pinger.cpp)
add_executable(Manager runManager.cpp Manager.h Manager.cpp ResourceAd.h ResourceAd.cpp
        Job.h Job.cpp SharedFiles.h SharedFiles.cpp pinger.h pinger.cpp Resource.h Resource.cpp)
add_executable(Execution runExecution.cpp Execution.h Execution.cpp ResourceAd.h ResourceAd.cpp
        Job.h Job.cpp SharedFiles.h SharedFiles.cpp pinger.h pinger.cpp Resource.h Resource.cpp)
add_executable(Storage runStorage.cpp Storage.h Storage.cpp ResourceAd.h ResourceAd.cpp
        Job.h Job.cpp SharedFiles.h SharedFiles.cpp pinger.h pinger.cpp Resource.h Resource.cpp)