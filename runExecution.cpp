//
// Created by Josh Szczesniak on 4/2/21.
//

#include "Execution.h"
#include "Resource.h"
using namespace std;

int main() {
    Execution e = Execution("/home/ubuntu/HTStorage/workingDir/execution1");
    cout << "Execution: Created the local Execution object." << endl;
    e.allowRecieveJob();
    e.createResourceAd(1024);

    char* ip = "18.222.110.161";

    e.advertiseToManager(ip, 8080);
    cout << "Execution: Sent the ResourceAd to the Manager at port 8080." << endl;

    e.executeJobs();

    Execution e2 = Execution("/home/ubuntu/HTStorage/workingDir/execution2");
    cout << "Execution: Created the 2nd local Execution object." << endl;
    e2.allowRecieveJob();
    e2.createResourceAd(1024);
    e2.advertiseToManager(ip, 8080);
    cout << "Execution: Sent the ResourceAd to the Manager at port 8080." << endl;

    e2.executeJobs();    
}
