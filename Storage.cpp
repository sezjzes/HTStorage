#include "Storage.h"
#include "pinger.h"
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
#include <stdio.h>
#include <string.h>
#include "SharedFiles.h"

static void* acceptNewStoragesLoop(void * args) {
    argumentsStorage *arg = (argumentsStorage *) args;
    Storage *s = arg->s;
    int soc_fd, server_fd;
    server_fd = arg->soc;
    struct sockaddr address;
    int addrlen = sizeof(address);

    while (true) {
        soc_fd = accept(server_fd, &address,
                        (socklen_t *) &addrlen);
        if (soc_fd < 0) {
            perror("accept");
            break;
        }
        SharedFiles::serialized j;
        read(soc_fd, (char*)&j, 4);
        read(soc_fd, (char*)&j+4, ntohl(j.packageSize));
        SharedFiles sf = SharedFiles((char *) &j);
        string dir = s->storageDir + "/" + to_string(s->folderNum++);
        mkdir(&dir[0], 0777);
        sf.setLocalPath(dir);
        sf.pullInFiles();
        sf.beginSharingFiles(false);
    }
}

void Storage::allowRecieveStorage() {

    pinger pi = pinger();
    strncpy(localIp, pi.getLocalIp(), 15);

    int server_fd;
    int port;
    int opt = 1;
    struct sockaddr_in address;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
               &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    for (port = 8080; 1; ++port) {
        address.sin_port = htons(port);
        if (::bind(server_fd, (struct sockaddr *) &address, (socklen_t)sizeof(address)) == 0){
            break;
        }
    }
    listen(server_fd, 3);
    pthread_t p;
    argumentsStorage * args = new argumentsStorage();
    args->s = this;
    args->soc = server_fd;
    pthread_create(&p, NULL,
                   acceptNewStoragesLoop, (void*)args);
    localPort = port;
}


void Storage::createResourceAd(int storage) {
    resourceAd = ResourceAd(-1, "storage", storage, 0,
                            storage, 0, localIp, localPort);
}

Storage::Storage(const string &storageDir) : storageDir(storageDir) {folderNum = 0;}
