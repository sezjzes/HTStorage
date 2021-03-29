//
// Created by Josh Szczesniak on 3/13/21.
//

#include "SharedFiles.h"
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <utility>
using namespace std;

SharedFiles::SharedFiles(string pathToSharedFiles, int writeSize) {
    localPath = pathToSharedFiles;
    size = writeSize;
}

void SharedFiles::beginSharingFiles() {
    Location local;
    local.ip = getLocalIp();
    local.readPort = allowRead();
    local.writePort = allowWrite();
    locations.push_back(local);
    //todo: share locations to other copies of the server
}

char *SharedFiles::getLocalIp() {
    //todo: make this not local host
    return "127.0.0.1";
}
struct arguments{
    int soc;
    string localPath;
};

static void* readHelper(void *args){
    arguments* arg = (arguments*)args;
    int soc_fd = arg->soc;
    string localPath = arg->localPath;
    free(args);
    char fileName[2048];
    int i = 0;
    read(soc_fd, fileName + i, 1);
    while(fileName[i]){
        i++;
        int r = read(soc_fd, fileName + i, 1);
        if (r<1) continue;
    }
    string fn = fileName;
    string filePath = localPath + "/"+ fileName;
    int fd = open(&filePath[0], O_RDONLY, NULL);
    char buff[2048];
    while(1){
        int r=read(fd, buff, 2048);
        if(!r){
            break;
        }
        int s =0;
        while(s<r){
            s += write(soc_fd, buff+s, r-s);
        }
    }
    close(soc_fd);
    close(fd);
}

[[noreturn]] static void* acceptingForReadHelper(void* args){
    int soc_fd, server_fd;
    arguments* arg = (arguments*)args;
    server_fd = arg->soc;
    struct sockaddr address;
    int addrlen = sizeof(address);
    pthread_t p;

    while (1){
        soc_fd = accept(server_fd, &address,
                        (socklen_t*)&addrlen);
        if (soc_fd < 0){
            perror("accept");
            break;
        }
        arg->soc = soc_fd;
        pthread_create(&p, NULL, readHelper, (void*) arg);
    }
}



int SharedFiles::allowRead() {
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
    arguments* args = (arguments *)(malloc(sizeof(arguments)));
    args->soc = server_fd;
    args->localPath = localPath;
    pthread_create(&p, NULL,
                   acceptingForReadHelper, (void*) args);
    return port;
}

static void* writeHelper(void *args){
    arguments* arg = (arguments*)args;
    int soc_fd = arg->soc;
    string localPath = arg->localPath;
    free(args);
    char fileName[2048];
    int i = 0;
    read(soc_fd, fileName + i, 1);
    while(fileName[i]){
        i++;
        int r = read(soc_fd, fileName + i, 1);
        if (r<1) continue;
    }
    string fn = fileName;
    string filePath = localPath + "/"+ fileName;
    cout << filePath;
    int fd = open(&filePath[0], O_WRONLY | O_CREAT | O_TRUNC, 777);
    char buff[2048];
    while(1){
        int r=read(soc_fd, buff, 2048);
        if(!r){
            break;
        }
        int s =0;
        while(s<r){
            s += write(fd, buff+s, r-s);
        }
    }
    close(soc_fd);
    close(fd);
}

[[noreturn]] static void* acceptingForWriteHelper(void* args){
    int soc_fd, server_fd;
    arguments* arg = (arguments*)args;
    server_fd = arg->soc;
    struct sockaddr address;
    int addrlen = sizeof(address);
    pthread_t p;

    while (1){
        write(0, "A", 1);
        soc_fd = accept(server_fd, &address,
                        (socklen_t*)&addrlen);
        write(0, "B", 1);
        if (soc_fd < 0){
            perror("accept");
        }
        arg->soc = soc_fd;
        pthread_create(&p, NULL, writeHelper, (void*) arg);
    }
}

int SharedFiles::allowWrite() {
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
    arguments* args = (arguments *)(malloc(sizeof(arguments)));
    args->soc = server_fd;
    args->localPath = localPath;
    pthread_create(&p, NULL,
                   acceptingForWriteHelper, (void*) args);
    return port;
}

int SharedFiles::openFileReadOnly(string fileName) {
    int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    for(Location l: locations){
        serv_addr.sin_port = htons(l.readPort);
        inet_pton(AF_INET, l.ip, &serv_addr.sin_addr);
        if (connect(soc_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == 0)
        {
            break;
        }
    }
    write(soc_fd, &fileName[0], fileName.length() + 1);
    return soc_fd;
}

int SharedFiles::openFileWriteOnly(string fileName) {
    int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    for(Location l: locations){
        serv_addr.sin_port = htons(l.writePort);
        inet_pton(AF_INET, l.ip, &serv_addr.sin_addr);
        if (connect(soc_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == 0)
        {
            break;
        }
    }
    write(soc_fd, &fileName[0], fileName.length() + 1);
    return soc_fd;
}

SharedFiles::~SharedFiles() = default;
