//
// Created by Josh Szczesniak on 4/3/21.
//
#include "Client.h"
using namespace std;

#include <sys/time.h>
int main() {
    struct timeval tp;
    gettimeofday( &tp, NULL);
    Client c = Client(
        "/home/mlev/Projects/HTStorage/tests/binaries/simpleRun.txt", 2,
        "/home/mlev/Projects/HTStorage/tests/files", 7);
    char* ip = "172.31.13.131";//"193.168.002.215";
    cout << "Client: Created the client at ip address " << ip << endl;
    c.sendJobToManger(ip, 8081);
    cout << "Client: Sent the job to the manager at port 8081." << endl;
    c.waitForComplete();

    struct timeval tv, tt;
    gettimeofday( &tv, NULL);
    timersub( &tv, &tp, &tt);
    int triptime = tt.tv_sec*1000000+(tt.tv_usec);
    cout << "Client: Total job time was "<< triptime/1000000.0 << " seconds." << endl;

}
