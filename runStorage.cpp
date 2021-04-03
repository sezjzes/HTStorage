//
// Created by Josh Szczesniak on 4/2/21.
//
#include "Storage.h"
int main() {
    Storage s = Storage("/Users/joshszczesniak/Desktop/525/HTStorage/workingDir/storage1");
    s.allowRecieveStorage();
    s.createResourceAd(1024);

    char* ip = "193.168.002.215";

    s.advertiseToManager(ip, 8080);
    Storage s2 = Storage("/Users/joshszczesniak/Desktop/525/HTStorage/workingDir/storage2");
    s2.allowRecieveStorage();
    s2.createResourceAd(1024);
    s2.advertiseToManager(ip, 8080);
    while(true);
}