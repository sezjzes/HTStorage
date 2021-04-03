//
// Created by Josh Szczesniak on 4/2/21.
//

#include "Execution.h"
#include "Resource.h"

int main() {
    Execution e = Execution();
    e.allowRecieveJob();
    e.createResourceAd(1024);

    char* ip = "193.168.002.215";

    e.advertiseToManager(ip, 8080);

    e.executeJobs();
}