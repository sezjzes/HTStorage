cmake_minimum_required(VERSION 3.17)
project(HTStorage)

set(CMAKE_CXX_STANDARD 17)
# add_subdirectory(Google_tests)
find_library(pthread pthread)
add_executable(HTStorage main.cpp pinger.h pinger.cpp)
add_executable(Manager runManager.cpp Manager.h Manager.cpp ResourceAd.h ResourceAd.cpp
        Job.h Job.cpp SharedFiles.h SharedFiles.cpp pinger.h pinger.cpp Resource.h Resource.cpp)
add_executable(Execution runExecution.cpp Execution.h Execution.cpp ResourceAd.h ResourceAd.cpp
        Job.h Job.cpp SharedFiles.h SharedFiles.cpp pinger.h pinger.cpp Resource.h Resource.cpp)
add_executable(Storage runStorage.cpp Storage.h Storage.cpp ResourceAd.h ResourceAd.cpp
        Job.h Job.cpp SharedFiles.h SharedFiles.cpp pinger.h pinger.cpp Resource.h Resource.cpp)

add_executable(Client runClient.cpp Client.h Client.cpp  ResourceAd.h ResourceAd.cpp
        Job.h Job.cpp SharedFiles.h SharedFiles.cpp pinger.h pinger.cpp Resource.h Resource.cpp)
target_link_libraries(Client pthread)
target_link_libraries(Storage pthread)
target_link_libraries(Execution pthread)
target_link_libraries(Manager pthread)
