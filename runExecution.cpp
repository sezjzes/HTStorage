//
// Created by Josh Szczesniak on 4/2/21.
//

#include "Execution.h"
#include "Resource.h"

int main() {
    Execution e = Execution("/Users/joshszczesniak/Desktop/525/HTStorage/workingDir/execution");
    e.allowRecieveJob();
    e.createResourceAd(1024);

    char* ip = "193.168.002.215";

    e.advertiseToManager(ip, 8080);

    e.executeJobs();
}