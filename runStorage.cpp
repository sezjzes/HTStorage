//
// Created by Josh Szczesniak on 4/2/21.
//
#include "Storage.h"
using namespace std;

int main() {
    Storage s = Storage("/home/ubuntu/HTStorage/workingDir/storage1");
    cout << "Storage: Created the primary Storage node." << endl;
    s.allowRecieveStorage();
    cout << "Storage: The primary Storage node is now ready now to receive copies of client files." << endl;
    s.createResourceAd(1024);

    char* ip = "3.134.85.120";

    s.advertiseToManager(ip, 8080);
    cout << "Storage: Sent the ResourceAd to the Manager." << endl;
    Storage s2 = Storage("/Users/joshszczesniak/Desktop/525/HTStorage/workingDir/storage2");
    cout << "Storage: Created the backup Storage node." << endl;
    s2.allowRecieveStorage();
    cout << "Storage: The backup Storage node is now ready now to receive copies of client files." << endl;
    s2.createResourceAd(1024);
    s2.advertiseToManager(ip, 8080);
    cout << "Storage: Sent the ResourceAd to the Manager." << endl;
    while(true);
}
