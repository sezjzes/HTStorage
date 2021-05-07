//
// Created by Josh Szczesniak on 4/2/21.
//

#include "Resource.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "pinger.h"
#include <unistd.h>
#include "Manager.h"
using namespace std;

void Resource::advertiseToManager(char* managerIp, int port) {
    cout << "Resource.cpp: Starting to advertise the resource to the manager." << endl;
    int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
    cout << "Resource.cpp: 1" << endl;
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    cout << "Resource.cpp: 2" << endl;
    serv_addr.sin_port = htons(port);
    cout << "Resource.cpp: 3" << endl;
    inet_pton(AF_INET, managerIp, &serv_addr.sin_addr);
    cout << "Resource.cpp: 4" << endl;
    connect(soc_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    cout << "Resource.cpp: 5" << endl;
    char buff[resourceAdSize];
    cout << "Resource.cpp: 6" << endl;
    resourceAd.serializeSelf(buff);
    cout << "Resource.cpp: 7" << endl;
    write(soc_fd, buff, resourceAdSize);
    cout << "Resource.cpp:  Send the ResourceAd to the Manager." << endl;

    int nrid;
    read(soc_fd, (char*) &nrid, 4);
    cout << "Resource.cpp: 8" << endl;
    resourceAd.setResourceID(ntohl(nrid));
    cout << "Resource.cpp: Set the resource ID in the ResourceAd as received from the Manager." << endl;
    int nsol;
    read(soc_fd, (char*) &nsol, 4);
    cout << "Resource.cpp: 9" << endl;
    int sol = ntohl(nsol);
    cout << "Resource.cpp: 10" << endl;
    resourceListEntry rl[sol];
    cout << "Resource.cpp: 11" << endl;
    read(soc_fd, (char*) rl,  sol * sizeof(resourceListEntry));
    cout << "Resource.cpp: 12" << endl;
    pinger p = pinger();
    cout << "Resource.cpp: 13" << endl;
    for(int i = 0; i < sol; i++){
        cout << "Resource.cpp: 14" << endl;
        rl[i].pingTime = p.ping(rl[i].ip);
        cout << "Resource.cpp: Set the local ping time for a resource." << endl;
    }
    cout << "Resource.cpp: 15" << endl;
    write(soc_fd, (char*) rl, sol * sizeof(resourceListEntry));
    cout << "Resource.cpp: Sent the ping times to the Manager." << endl;
}
