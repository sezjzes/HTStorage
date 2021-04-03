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

void Resource::advertiseToManager(char* managerIp, int port) {
    int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, managerIp, &serv_addr.sin_addr);
    connect(soc_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    char buff[resourceAdSize];
    resourceAd.serializeSelf(buff);
    write(soc_fd, buff, resourceAdSize);

    int nrid;
    read(soc_fd, (char*) &nrid, 4);
    resourceAd.setResourceID(ntohl(nrid));
    int nsol;
    read(soc_fd, (char*) &nsol, 4);
    int sol = ntohl(nsol);
    resourceListEntry rl[sol];
    read(soc_fd, (char*) rl,  sol * sizeof(resourceListEntry));
    pinger p = pinger();

    for(int i = 0; i < sol; i++){
        rl[i].pingTime = p.ping(rl[i].ip);
    }
    write(soc_fd, (char*) rl, sol * sizeof(resourceListEntry));
}
