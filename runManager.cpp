//
// Created by Josh Szczesniak on 4/2/21.
//
#include <unistd.h>
#include "Manager.h"
int main() {
    Manager m = Manager();
    m.acceptNewResourceAds();

    m.acceptNewJobs();

    m.assignJobs();

    while(true){sleep(100);}
}