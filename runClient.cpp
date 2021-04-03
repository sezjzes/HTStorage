//
// Created by Josh Szczesniak on 4/3/21.
//
#include "Client.h"

#include <sys/time.h>
int main() {
    struct timeval tp;
    gettimeofday( &tp, NULL);
    Client c = Client(
            "/Users/joshszczesniak/Desktop/525/HTStorage/tests/binaries/simpleRun.txt", 2,
            "/Users/joshszczesniak/Desktop/525/HTStorage/tests/files", 7);
    char* ip = "193.168.002.215";
    c.sendJobToManger(ip, 8081);
    c.waitForComplete();

    struct timeval tv, tt;
    gettimeofday( &tv, NULL);
    timersub( &tv, &tp, &tt);
    int triptime = tt.tv_sec*1000000+(tt.tv_usec);
    std::cout<<triptime/1000000.0<<std::endl;
    while (true);

}
