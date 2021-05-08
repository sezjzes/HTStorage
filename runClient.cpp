//
// Created by Josh Szczesniak on 4/3/21.
//
#include "Client.h"
using namespace std;

#include <sys/time.h>
#include <string>
#include <vector>

int main() {
    struct timeval tp;
    gettimeofday( &tp, NULL);
    int n = 5;
    vector<Client*> clients;
    for(int i = 0; i < n; i++) {
        Client* c= new Client(
                "/home/ubuntu/HTStorage/tests/binaries/simpleRun.txt", 2,
                "/home/ubuntu/HTStorage/tests/files/" + to_string(i), 7);
        char *ip = "3.134.85.120";
        cout << "Client: Created the client at ip address " << ip << endl;
        c->sendJobToManger(ip, 8081);
        cout << "Client: Sent the job to the manager at port 8081." << endl;
        clients.push_back(c);
    }
    for(auto &c: clients) {
        c->waitForComplete();
    }

    struct timeval tv, tt;
    gettimeofday( &tv, NULL);
    timersub( &tv, &tp, &tt);
    int triptime = tt.tv_sec*1000000+(tt.tv_usec);
    cout << "Client: Total job time was "<< triptime/1000000.0 << " seconds." << endl;

}
