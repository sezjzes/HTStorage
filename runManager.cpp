//
// Created by Josh Szczesniak on 4/2/21.
//
#include <unistd.h>
#include "Manager.h"
using namespace std;

int main() {
    Manager m = Manager();
    cout << "Created the Manager object." << endl;
    m.acceptNewResourceAds();
    cout << "Manager: Accepted new ResourceAds." << endl;
    m.acceptNewJobs();
    cout << "Manager: Accepted new Jobs." << endl;
    m.assignJobs();
    cout << "Manager: Assigned jobs to available resources." << endl;

    while(true){sleep(100);}
}
