//
// Created by Josh Szczesniak on 3/13/21.
//

#include "Job.h"
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
#include <sys/poll.h>
#include <map>

Job::Job(SharedFiles files, string path_to_code, int compute) {
    shared_files = files;
    PathToLocalFile = path_to_code;
    required_compute = compute;
}

Job::~Job() {}


int Job::getRequiredStorage() {
    return shared_files.getSize();
}

int Job::getRequiredCompute() {
    return required_compute;
}

Job::Job() {}

void sendFile(int soc_fd, string path){
    string filePath = path;
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
    close(fd);
    close(soc_fd);

}

static void* acceptingForAllowPullJob(void* args){
    int soc_fd, server_fd;
    arguments* arg = (arguments*)args;
    server_fd = arg->soc;
    string path = arg->localPath;
    struct sockaddr address;
    int addrlen = sizeof(address);

    soc_fd = accept(server_fd, &address,
                    (socklen_t*)&addrlen);
    if (soc_fd < 0){
        perror("accept");
    }
    arg->soc = soc_fd;
    sendFile(soc_fd, path);
};

void Job::AllowPullJob() {
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
    pinger pi = pinger();
    strncpy(binaryIp, pi.getLocalIp(), 15);
    binaryPort = port;
    pthread_t p;
    listen(server_fd, 3);
    arguments* args = new arguments;
    args->soc = server_fd;
    args->localPath = PathToLocalFile;
    pthread_create(&p, NULL, acceptingForAllowPullJob, (void*) args);
}

void Job::PullJob(string path) {
    PathToLocalFile = path;
    int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(binaryPort);
    inet_pton(AF_INET, binaryIp, &serv_addr.sin_addr);
    connect(soc_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    char buff[2048];
    int fd = open((char *)&path[0], O_WRONLY | O_CREAT | O_TRUNC, 0777);
    while(true){
        int r = read(soc_fd, buff, 2048);
        if (!r){
            break;
        }
        int s =0;
        while(s<r){
            s += write(fd, buff+s, r-s);
        }
    }
    close(fd);
    close(soc_fd);
}

SharedFiles Job::GetStorage() {
    return shared_files;
}

void Job::Run() {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t r;

    fp = fopen((char*) &PathToLocalFile[0], "r");

    map<string, int> files = map<string, int>();
    while ((r = getline(&line, &len, fp)) != -1) {
        if(line[r-1] == '\n'){line[r-1] = 0;}

        char buff[2048];
        if(!strncmp(line, "compute", 7)){
            int i = atoi(line+8);
            sleep(i);
            continue;
        }
        if(!strncmp(line, "openWrite", 9)){
            string str = line+10;
            files[str] = shared_files.openFileWriteOnly(&str[0]);
            continue;
        }
        if(!strncmp(line, "openRead", 8)){
            string str = line+9;
            files[str] = shared_files.openFileReadOnly(&str[0]);
            continue;
        }
        if(!strncmp(line, "write", 5)){
            char *token = strtok(line +6, " ");
            string str = token;
            int a = atoi(strtok(NULL, " "));
            write(files[str], strtok(NULL, " "), a);
            continue;
        }
        if(!strncmp(line, "read", 4)){
            char *token = strtok(line +5, " ");
            string str = token;
            int a = atoi(strtok(NULL, " "));
            read(files[str], buff, a);
            continue;
        }
    }

    fclose(fp);
}

int Job::SerializeSelf(char *buff) {
    serializedJob * s = (serializedJob*)buff;
    int ps = 4+4+15;
    s->required_compute = htonl(required_compute);
    s->binaryPort = htonl(binaryPort);
    strncpy(s->binaryIp, binaryIp,15);
    ps += shared_files.serializeSelf((char *) &s->sf);
    s->packageSize = htonl(ps);
    return ps + 4;
}

Job::Job(char *serializedVersion) {
    serializedJob * s = (serializedJob*)serializedVersion;
    required_compute = ntohl(s->required_compute);
    binaryPort = ntohl(s->binaryPort);
    strncpy(binaryIp, s->binaryIp,15);
    shared_files = SharedFiles((char*)&s->sf);
}
