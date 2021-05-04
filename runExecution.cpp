//
// Created by Josh Szczesniak on 4/2/21.
//

#include "Execution.h"
#include "Resource.h"
using namespace std;

int main() {
    Execution e = Execution("/home/mlev/Projects/HTStorage/workingDir/execution");
    cout << "Execution: Created the local Execution object." << endl;
    e.allowRecieveJob();
    e.createResourceAd(1024);

    char* ip = "127.0.1.1"; //"193.168.002.215";

    e.advertiseToManager(ip, 8080);
    cout << "Execution: Sent the ResourceAd to the Manager at port 8080." << endl;

    e.executeJobs();
}
