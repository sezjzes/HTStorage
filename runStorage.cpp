//
// Created by Josh Szczesniak on 4/2/21.
//
#include "Storage.h"
int main() {
    Storage s = Storage();
    s.allowRecieveStorage();
    s.createResourceAd(1024);

    char* ip = "193.168.002.215";

    s.advertiseToManager(ip, 8080);
}