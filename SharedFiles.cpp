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
#include <string>
#include <iostream>
#include <filesystem>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "pinger.h"
#include <unistd.h>
#include <cstring>
#include <sys/poll.h>
#include <pthread.h>

namespace fs = std::filesystem;

#include <utility>
using namespace std;

SharedFiles::SharedFiles(string pathToSharedFiles, int writeSize) {
    localPath = std::move(pathToSharedFiles);
    size = writeSize;
    syncfd = 0;
    for (const auto & entry : fs::directory_iterator(localPath)) {
        if(entry.is_directory()){
            continue;
        }
        size += entry.file_size();
    }
    handelSyncLocations();
    beginSharingFiles(true);

    isClient = true;
    complete = false;
}


void SharedFiles::beginSharingFiles(bool isClient) {
    cout << "SharedFiles.cpp: Beginning to share files." << endl;
    Location local;
    strncpy(local.ip, getLocalIp(), 15);
    local.readPort = allowRead();
    local.writePort = allowWrite();
    local.pullInPort = allowPullInFiles();
    local.writeOutPort = allowWriteOutFiles();
    local.completePort = allowComplete();
    local.isClient = isClient;
    if(!isClient){
        connectToSyncLocations();
        cout << "SharedFiles.cpp:  It is not a client, so connecting to the sync locations." << endl;
    }
    sendNewLocation(local);
    locations.push_back(local);
    //todo: share locations to other copies of the server
}

char *SharedFiles::getLocalIp() {
    //todo: make this not local host
    char host[256];
    char *ip;
    struct hostent *host_entry;
    int hostname;
    hostname = gethostname(host, sizeof(host)); //find the host name
    host_entry = gethostbyname(host); //find host information
    ip = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));
    return ip;
}


static void* readHelper(void *args){
    arguments* arg = (arguments*)args;
    int soc_fd = arg->soc;
    string localPath = arg->localPath;
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
    while(true){
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

    while (true){
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
    for (port = 8080; true; ++port) {
        address.sin_port = htons(port);
        if (::bind(server_fd, (struct sockaddr *) &address, (socklen_t)sizeof(address)) == 0){
            break;
        }
    }
    listen(server_fd, 3);
    pthread_t p;
    arguments* args = new arguments;
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
    int fd = open(&filePath[0], O_WRONLY | O_CREAT | O_TRUNC, 0777);

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
        soc_fd = accept(server_fd, &address,
                        (socklen_t*)&addrlen);

        if (soc_fd < 0){
            perror("accept");
        }
        arg->soc = soc_fd;
        pthread_create(&p, NULL, writeHelper, args);
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
    arguments* args = new arguments;
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

void SharedFiles::setLocalPath(string lp) {
    localPath = lp;
}

void sendAllFiles(int soc_fd, string path){
    cout << "SharedFiles.cpp: Starting to send out all the files." << endl;
    for (const auto & entry : fs::directory_iterator(path)) {

        if (entry.is_directory()){
            continue;
        }
        string fn = entry.path().filename();
        string filePath = entry.path();
        int fd = open(&filePath[0], O_RDONLY, NULL);
        char buff[2048];
        int strlen = htonl(fn.length());
        write(soc_fd, (char *)&strlen, 4);
        write(soc_fd, &fn[0], fn.length());
        int fileSize = htonl(entry.file_size());
        write(soc_fd, (char *)&fileSize, 4);
        while(true){
            int r=read(fd, buff, 2048);
            if(!r){
                break;
            }
            int s =0;
            while(s<r){
                s += write(soc_fd, buff+s, r-s);
            }
        }
        close(fd);
    }
    close(soc_fd);
    cout << "SharedFiles.cpp: Finished sending out all of the files." << endl;
}

[[noreturn]] static void* acceptingForAllowPullInFilesHelper(void* args){
    int soc_fd, server_fd;
    arguments* arg = (arguments*)args;
    server_fd = arg->soc;
    string path = arg->localPath;
    struct sockaddr address;
    int addrlen = sizeof(address);

    while (true){
        soc_fd = accept(server_fd, &address,
                        (socklen_t*)&addrlen);
        if (soc_fd < 0){
            perror("accept");
        }
        arg->soc = soc_fd;
        sendAllFiles(soc_fd, path);
    }
}

int SharedFiles::allowPullInFiles() {
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
    arguments* args = new arguments;
    args->soc = server_fd;
    args->localPath = localPath;
    pthread_create(&p, NULL,
                   acceptingForAllowPullInFilesHelper, (void*) args);
    return port;
}

void SharedFiles::pullInFiles() {
    cout << "SharedFiles.cpp: Beginning to pull in files." << endl;
    int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    for(Location l: locations){
        serv_addr.sin_port = htons(l.pullInPort);
        inet_pton(AF_INET, l.ip, &serv_addr.sin_addr);
        if (connect(soc_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == 0)
        {
            break;
        }
    }
    while(true){
        char buff[2048];
        char filename[2048] = {0};
        int r = read(soc_fd, buff, 4);
        if(!r){
            break;
        }
        int strlen = ntohl(*(int *)buff);
        read(soc_fd, filename, strlen);
        string fn = filename;
        read(soc_fd, buff, 4);
        int toRead = ntohl(*(int *)buff);
        string filePath = localPath + "/"+ fn;
        int fd = open(&filePath[0], O_WRONLY | O_CREAT | O_TRUNC, 0777);
        while(toRead){
            r = read(soc_fd, buff, min(2048, toRead));
            toRead -= r;
            int s =0;
            while(s<r){
                s += write(fd, buff+s, r-s);
            }
        }
        close(fd);
    }
    close(soc_fd);
    cout << "SharedFiles.cpp: Finished pulling in all of the files." << endl;
}

void recvAllFiles(int soc_fd, string path){
    cout << "SharedFiles.cpp: Beginning to receive all files." << endl;
    while(true){
        char buff[2048];
        char filename[2048] = {0};
        int r = read(soc_fd, buff, 4);
        if(!r){
            break;
        }
        int strlen = ntohl(*(int *)buff);
        read(soc_fd, filename, strlen);
        string fn = filename;
        read(soc_fd, buff, 4);
        int toRead = ntohl(*(int *)buff);
        string filePath = path + "/"+ fn;
        int fd = open(&filePath[0], O_WRONLY | O_CREAT | O_TRUNC, 0777);
        while(toRead){
            r = read(soc_fd, buff, min(2048, toRead));
            toRead -= r;
            int s =0;
            while(s<r){
                s += write(fd, buff+s, r-s);
            }
        }
        close(fd);
    }
    close(soc_fd);
    cout << "SharedFiles.cpp: Finished receiving all files." << endl;
}

[[noreturn]] static void* acceptingForAllowWriteOutFilesHelper(void* args){
    int soc_fd, server_fd;
    arguments* arg = (arguments*)args;
    server_fd = arg->soc;
    string path = arg->localPath;
    struct sockaddr address;
    int addrlen = sizeof(address);

    while (true){
        soc_fd = accept(server_fd, &address,
                        (socklen_t*)&addrlen);
        if (soc_fd < 0){
            perror("accept");
        }
        arg->soc = soc_fd;
        recvAllFiles(soc_fd, path);
    }
}

int SharedFiles::allowWriteOutFiles() {
    int server_fd;
    int port;
    int opt = 1;
    struct sockaddr_in address;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
               &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    for (port = 8080; true; ++port) {
        address.sin_port = htons(port);
        if (::bind(server_fd, (struct sockaddr *) &address, (socklen_t)sizeof(address)) == 0){
            break;
        }
    }
    listen(server_fd, 3);
    pthread_t p;
    arguments* args = new arguments;
    args->soc = server_fd;
    args->localPath = localPath;
    pthread_create(&p, NULL,
                   acceptingForAllowWriteOutFilesHelper, (void*) args);
    return port;
}

void SharedFiles::writeOutFiles() {
    cout << "SharedFiles.cpp: Beginning to write out the files." << endl;
    int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    for(Location l: locations){
        if (!l.isClient){
            continue;
        }
        serv_addr.sin_port = htons(l.writeOutPort);
        inet_pton(AF_INET, l.ip, &serv_addr.sin_addr);
        if (connect(soc_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == 0)
        {
            break;
        }
    }
    for (const auto & entry : fs::directory_iterator(localPath)) {
        if (entry.is_directory()){
            continue;
        }
        string fn = entry.path().filename();
        string filePath = entry.path();
        int fd = open(&filePath[0], O_RDONLY, NULL);
        char buff[2048];
        int strlen = htonl(fn.length());
        write(soc_fd, (char *)&strlen, 4);
        write(soc_fd, &fn[0], fn.length());
        int fileSize = htonl(entry.file_size());
        write(soc_fd, (char *)&fileSize, 4);
        while(true){
            int r=read(fd, buff, 2048);
            if(!r){
                break;
            }
            int s =0;
            while(s<r){
                s += write(soc_fd, buff+s, r-s);
            }
        }
        close(fd);
    }
    close(soc_fd);
    cout << "SharedFiles.cpp:  Finished writing out the files." << endl;
}
static void* acceptComplete(void* args){
    cout << "SharedFiles.cpp: Beginning to accept Complete." << endl;
    int soc_fd, server_fd;
    arguments* arg = (arguments*)args;
    server_fd = arg->soc;
    string path = arg->localPath;
    struct sockaddr address;
    SharedFiles &sf = *arg->sf;
    int addrlen = sizeof(address);

    soc_fd = accept(server_fd, &address,
                    (socklen_t *) &addrlen);
    cout<<"close"<<endl;
    if(!sf.isClient) {
        cout << "SharedFiles.cpp: The shared files object is not at a client." << endl;
        sf.writeOutFiles();
        cout << "SharedFiles.cpp: The shared files have been written out." << endl;
        fs::remove_all(sf.localPath);
        cout << "SharedFiles.cpp: The local version of the sharedfiles have been deleted." << endl;
    }
    sf.complete = true;
    write(soc_fd, "c", 1);
    cout<<"closed"<<endl;
    close(soc_fd);
    cout << "SharedFiles.cpp: Finished accepting the Complete." << endl;
}

int SharedFiles::allowComplete() {
    int server_fd;
    int port;
    int opt = 1;
    struct sockaddr_in address;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
               &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    for (port = 8080; true; ++port) {
        address.sin_port = htons(port);
        if (::bind(server_fd, (struct sockaddr *) &address, (socklen_t)sizeof(address)) == 0){
            break;
        }
    }
    listen(server_fd, 3);
    pthread_t p;
    arguments* args = new arguments;
    args->soc = server_fd;
    args->localPath = localPath;
    args->sf = this;
    pthread_create(&p, NULL,
                   acceptComplete, (void*) args);
    return port;
}

void SharedFiles::sendComplete() {
    cout << "SharedFiles.cpp: Beginning to send out the Complete." << endl;
    complete = true;
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    for(Location l: locations){
        int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
        if(l.isClient){
            continue;
        }
        cout<<"close" << l.completePort <<endl;
        serv_addr.sin_port = htons(l.completePort);
        inet_pton(AF_INET, l.ip, &serv_addr.sin_addr);
        if (connect(soc_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
        {
            perror("hi");
            continue;
        }
        cout<<"closed"<<endl;
        char c;
        read(soc_fd, &c, 1);
        close(soc_fd);
    }
    for(Location l: locations){
        int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
        if(!l.isClient){
            continue;
        }
        cout<<"end"<<endl;
        serv_addr.sin_port = htons(l.completePort);
        inet_pton(AF_INET, l.ip, &serv_addr.sin_addr);
        if (connect(soc_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
        {
            perror("hi");
            continue;
        }
        cout<<"ended"<<endl;
        char c;
        read(soc_fd, &c, 1);

    }
    cout << "SharedFiles.cpp: Finished sending out the Complete." << endl;
}

int SharedFiles::getSize() {
    return size;
}

int SharedFiles::serializeSelf(char *buff) {
    serialized * s = (serialized*)buff;
    int ps = 4+4+15;
    s->size = htonl(size);
    s->syncPort = htonl(syncPort);
    strncpy(s->syncIp, syncIP,15);
    ps += serializeLocationList((char *)&s->locationList);
    s->packageSize = htonl(ps);
    return ps + 4;
}

int SharedFiles::serializeLocationList(char *buff) {
    serializedLocationList * sll = (serializedLocationList *)buff;
    int ps = 4;

    sll->numLocations = htonl(locations.size());
    int i = 0;
    for(Location l: locations){
        ps += serializeLocation((char *) &sll->locations[i], l);
        i++;
    }
    sll->packageSize = htonl(ps);
    return ps + 4;
}

int SharedFiles::serializeLocation(char *buff, Location l) {
    serializedLocation* sl = (serializedLocation*) buff;
    strncpy(sl->ip, l.ip, 15);
    sl->isClient = l.isClient;
    sl->pullInPort = htonl(l.pullInPort);
    sl->writeOutPort = htonl(l.writeOutPort);
    sl->writePort = htonl(l.writePort);
    sl->readPort = htonl(l.readPort);
    sl->completePort = htonl(l.completePort);
    return 36;
}

Location SharedFiles::unserializeLocation(serializedLocation *sl) {
    Location l;
    strncpy(l.ip, sl->ip, 15);
    l.isClient = sl->isClient;
    l.pullInPort  = ntohl(sl->pullInPort);
    l.writeOutPort = ntohl(sl->writeOutPort);
    l.writePort = ntohl(sl->writePort);
    l.readPort = ntohl(sl->readPort);
    l.completePort = ntohl(sl->completePort);
    return l;
}

void SharedFiles::unserializeLocationList(serializedLocationList * sll) {
    locations.clear();

    for(int i = 0; i < ntohl(sll->numLocations); i++){

        locations.push_back(unserializeLocation(&sll->locations[i]));
    }

    sortLocations();
}

SharedFiles::SharedFiles(char *serializedVersion) {
    serialized* s = (serialized*)serializedVersion;
    size = ntohl(s->size);
    syncPort = ntohl(s->syncPort);
    strncpy(syncIP, s->syncIp,15);
    unserializeLocationList(&s->locationList);

    isClient = false;
    complete = false;
}

static bool compare_locations (const Location& first, const Location& second)
{
    return first.ping < second.ping;
}

void SharedFiles::sortLocations() {
    pinger p = pinger();
    for(Location& l: locations){
        l.ping = p.ping(l.ip);
    }
    locations.sort(compare_locations);
}

[[noreturn]] static void* acceptingSyncLocations(void* args){
    int soc_fd, server_fd;
    arguments* arg = (arguments*)args;
    server_fd = arg->soc;
    string path = arg->localPath;
    struct sockaddr address;
    SharedFiles &sf = *arg->sf;
    int addrlen = sizeof(address);
    struct pollfd fds[10];
    int numfds = 1;
    fds[0].fd = server_fd;
    fds[0].events = POLLIN;
    fds[0].revents = 0;
    while (true){
        bool updated = false;
        poll(fds, numfds, -1);
        if(fds[0].revents & POLLIN ) {
            updated = true;
            soc_fd = accept(server_fd, &address,
                            (socklen_t *) &addrlen);
            if (soc_fd < 0) {
                perror("accept");
            }
            fds[numfds].fd = soc_fd;
            fds[numfds].events = POLLIN;
            fds[numfds].revents = 0;
            numfds++;
        }
        for(int i = 1; i < numfds; i++){
            if(fds[i].revents & POLLIN ){
                updated = true;

                SharedFiles::serializedLocation sl;
                read(fds[i].fd, (char*)&sl, 36);
                Location l = sf.unserializeLocation(&sl);
                sf.addLocation(l);
            }
            fds[i].revents = 0;
        }
        if(updated){
            for(int i = 1; i < numfds; i++) {
                char buff[2048];
                write(fds[i].fd, buff, sf.serializeLocationList(buff));
            }
        }

    }
}

void SharedFiles::handelSyncLocations() {
    int server_fd;
    int port;
    int opt = 1;
    struct sockaddr_in address;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
               &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    for (port = 8080; true; ++port) {
        address.sin_port = htons(port);
        if (::bind(server_fd, (struct sockaddr *) &address, (socklen_t)sizeof(address)) == 0){
            break;
        }
    }
    listen(server_fd, 3);
    pthread_t p;
    arguments* args = new arguments;
    args->soc = server_fd;
    args->localPath = localPath;
    args->sf = this;
    pthread_create(&p, NULL,
                   acceptingSyncLocations, (void*) args);
    strncpy(syncIP, getLocalIp(), 15);
    syncPort =  port;
}

static void* recvUpdates(void* self){
    SharedFiles &sf = *((SharedFiles *) self);
    SharedFiles::serializedLocationList sll;
    while (true){
        int r = read(sf.getSyncfd(), (void *) &sll, 4);
        if (r == 0){break;}
        r = read(sf.getSyncfd(), (void *) &(sll.numLocations), ntohl(sll.packageSize));
        if (r == 0){break;}
        sf.unserializeLocationList(&sll);
    }
}

void SharedFiles::connectToSyncLocations() {
    int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(syncPort);
    inet_pton(AF_INET, syncIP, &serv_addr.sin_addr);
    while(connect(soc_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)));
    syncfd = soc_fd;
    pthread_t p;
    pthread_create(&p, NULL,
                   recvUpdates, (void*) this);
}

void SharedFiles::sendNewLocation(Location l) {
    char buff[2048];
    if(!syncfd){return;}
    write(syncfd, buff, serializeLocation(buff, l));
}

int SharedFiles::getSyncfd() const {
    return syncfd;
}

void SharedFiles::addLocation(Location l){
    locations.push_back(l);
}

SharedFiles::SharedFiles() {localPath = "no";}



SharedFiles::~SharedFiles() { }
