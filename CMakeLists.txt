cmake_minimum_required(VERSION 3.17)
project(HTStorage)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(Google_tests)

include_directories(/Users/joshszczesniak/Desktop/ns-allinone-3.33/ns-3.33/build /Users/joshszczesniak/Desktop/ns-allinone-3.33/ns-3.33/build/lib)
link_directories(/Users/joshszczesniak/Desktop/ns-allinone-3.33/ns-3.33/build /Users/joshszczesniak/Desktop/ns-allinone-3.33/ns-3.33/build/lib)

project(NS-3SIMULATION)


add_executable(HTStorage main.cpp pinger.h pinger.cpp)
add_executable(Manager runManager.cpp Manager.h Manager.cpp ResourceAd.h ResourceAd.cpp
        Job.h Job.cpp SharedFiles.h SharedFiles.cpp pinger.h pinger.cpp Resource.h Resource.cpp)
add_executable(Execution runExecution.cpp Execution.h Execution.cpp ResourceAd.h ResourceAd.cpp
        Job.h Job.cpp SharedFiles.h SharedFiles.cpp pinger.h pinger.cpp Resource.h Resource.cpp)
add_executable(Storage runStorage.cpp Storage.h Storage.cpp ResourceAd.h ResourceAd.cpp
        Job.h Job.cpp SharedFiles.h SharedFiles.cpp pinger.h pinger.cpp Resource.h Resource.cpp)

add_executable(Client runClient.cpp Client.h Client.cpp  ResourceAd.h ResourceAd.cpp
        Job.h Job.cpp SharedFiles.h SharedFiles.cpp pinger.h pinger.cpp Resource.h Resource.cpp)

add_executable(Local localRun.cpp)

add_executable(NS-3SIMULATION runSimulation.cpp managerApp.cpp managerApp.h clientApp.cpp clientApp.h Manager.h Manager.cpp Execution.h Execution.cpp ResourceAd.h ResourceAd.cpp
        Job.h Job.cpp SharedFiles.h SharedFiles.cpp pinger.h pinger.cpp Resource.h Resource.cpp Client.h Client.cpp)

target_link_libraries(NS-3SIMULATION
        ns3.33-core-debug
        ns3.33-wave-debug
        ns3.33-wifi-debug
        ns3.33-netanim-debug
        ns3.33-network-debug
        ns3.33-internet-debug
        ns3.33-mobility-debug
#        ns3.33-visualizer-debug
        ns3.33-applications-debug
        ns3.33-point-to-point-debug
        ns3.33-csma-debug
        )