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
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, managerIp, &serv_addr.sin_addr);
    connect(soc_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    char buff[resourceAdSize];
    resourceAd.serializeSelf(buff);
    write(soc_fd, buff, resourceAdSize);
    cout << "Resource.cpp:  Send the ResourceAd to the Manager." << endl;

    int nrid;
    read(soc_fd, (char*) &nrid, 4);
    resourceAd.setResourceID(ntohl(nrid));
    cout << "Resource.cpp: Set the resource ID in the ResourceAd as received from the Manager." << endl;
    int nsol;
    read(soc_fd, (char*) &nsol, 4);
    int sol = ntohl(nsol);
    resourceListEntry rl[sol];
    read(soc_fd, (char*) rl,  sol * sizeof(resourceListEntry));
    pinger p = pinger();

    for(int i = 0; i < sol; i++){
        rl[i].pingTime = p.ping(rl[i].ip);
        cout << "Resource.cpp: Set the local ping time for a resource." << endl;
    }
    write(soc_fd, (char*) rl, sol * sizeof(resourceListEntry));
    cout << "Resource.cpp: Sent the ping times to the Manager." << endl;
}
