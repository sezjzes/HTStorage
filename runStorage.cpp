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

    char* ip = "18.222.110.161";

    s.advertiseToManager(ip, 8080);
    cout << "Storage: Sent the ResourceAd to the Manager." << endl;
    Storage s2 = Storage("/home/ubuntu/HTStorage/workingDir/storage2");
    cout << "Storage: Created the backup Storage node." << endl;
    s2.allowRecieveStorage();
    cout << "Storage: The backup Storage node is now ready now to receive copies of client files." << endl;
    s2.createResourceAd(1024);
    s2.advertiseToManager(ip, 8080);
    cout << "Storage: Sent the ResourceAd to the Manager." << endl;

    Storage s3 = Storage("/home/ubuntu/HTStorage/workingDir/storage3");
    cout << "Storage: Created the 2nd primary Storage node." << endl;
    s3.allowRecieveStorage();
    cout << "Storage: The 2nd primary Storage node is now ready now to receive copies of client files." << endl;
    s3.createResourceAd(1024);
    s3.advertiseToManager(ip, 8080);
    cout << "Storage: Sent the ResourceAd to the Manager." << endl;
    Storage s4 = Storage("/home/ubuntu/HTStorage/workingDir/storage4");
    cout << "Storage: Created the 2nd backup Storage node." << endl;
    s4.allowRecieveStorage();
    cout << "Storage: The 2nd backup Storage node is now ready now to receive copies of client files." << endl;
    s4.createResourceAd(1024);
    s4.advertiseToManager(ip, 8080);
    cout << "Storage: Sent the ResourceAd to the Manager." << endl;

    while(true);
}
