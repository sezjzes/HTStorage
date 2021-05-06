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
        "/home/ubuntu/HTStorage/tests/binaries/simpleRun.txt", 2,
        "/home/ubuntu/HTStorage/tests/files", 7);
    char* ip = "18.222.110.161";
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