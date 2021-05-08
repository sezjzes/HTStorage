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
#include <cstring>
#include <map>
#include <pthread.h>
using namespace std;

Job::Job(string files, int writeSize, string path_to_code, int compute) : shared_files(files, writeSize) {
    PathToLocalFile = path_to_code;
    required_compute = compute;
}

Job::~Job() {}


int Job::getRequiredStorage() {
    cout << "Job.cpp: 1" << endl;
    return shared_files.getSize();
    cout << "Job.cpp: 2" << endl;
}

int Job::getRequiredCompute() {
    cout << "Job.cpp: 3" << endl;
    return required_compute;
    cout << "Job.cpp: 4" << endl;
}

Job::Job() {}

void sendFile(int soc_fd, string path){
    cout << "Job.cpp: Starting to send the file." << endl;
    string filePath = path;
    cout << "Job.cpp: 5" << endl;
    int fd = open(&filePath[0], O_RDONLY, NULL);
    cout << "Job.cpp: 6" << endl;
    char buff[2048];
    cout << "Job.cpp: 7" << endl;
    while(true){
        cout << "Job.cpp: 8" << endl;
        int r=read(fd, buff, 2048);
        cout << "Job.cpp: 9" << endl;
        if(!r){
            cout << "Job.cpp: 10" << endl;
            break;
        }
        cout << "Job.cpp: 11" << endl;
        int s =0;
        cout << "Job.cpp: 12" << endl;
        while(s<r){
            cout << "Job.cpp: 13" << endl;
            s += write(soc_fd, buff+s, r-s);
            cout << "Job.cpp: 14" << endl;
        }
        cout << "Job.cpp: 15" << endl;
    }
    cout << "Job.cpp: 16" << endl;
    close(fd);
    cout << "Job.cpp: 17" << endl;
    close(soc_fd);
    cout << "Job.cpp:  Finished sending the file." << endl;
}

static void* acceptingForAllowPullJob(void* args){
    cout << "Job.cpp: 18" << endl;
    int soc_fd, server_fd;
    arguments* arg = (arguments*)args;
    cout << "Job.cpp: 19" << endl;
    server_fd = arg->soc;
    cout << "Job.cpp: 20" << endl;
    string path = arg->localPath;
    cout << "Job.cpp: 21" << endl;
    struct sockaddr address;
    int addrlen = sizeof(address);
    cout << "Job.cpp: 22" << endl;

    soc_fd = accept(server_fd, &address,
                    (socklen_t*)&addrlen);
    cout << "Job.cpp: 23" << endl;
    if (soc_fd < 0){
        cout << "Job.cpp: 24" << endl;
        perror("accept");
        cout << "Job.cpp: 25" << endl;
    }
    cout << "Job.cpp: 26" << endl;
    arg->soc = soc_fd;
    cout << "Job.cpp: 27" << endl;
    sendFile(soc_fd, path);
    cout << "Job.cpp: 28" << endl;
};

void Job::AllowPullJob() {
    cout << "Job.cpp: 29" << endl;
    int server_fd;
    int port;
    int opt = 1;
    struct sockaddr_in address;
    cout << "Job.cpp: 30" << endl;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    cout << "Job.cpp: 31" << endl;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
               &opt, sizeof(opt));
    cout << "Job.cpp: 32" << endl;
    address.sin_family = AF_INET;
    cout << "Job.cpp: 33" << endl;
    address.sin_addr.s_addr = INADDR_ANY;
    cout << "Job.cpp: 34" << endl;
    for (port = 8080; true; ++port) {
        cout << "Job.cpp: 35" << endl;
        address.sin_port = htons(port);
        cout << "Job.cpp: 36" << endl;
        if (::bind(server_fd, (struct sockaddr *) &address, (socklen_t)sizeof(address)) == 0){
            cout << "Job.cpp: 37" << endl;
            break;
        }
        cout << "Job.cpp: 38" << endl;
    }
    cout << "Job.cpp: 39" << endl;
    pinger pi = pinger();
    cout << "Job.cpp: 40" << endl;
    strncpy(binaryIp, pi.getLocalIp(), 15);
    cout << "Job.cpp: 41" << endl;
    binaryPort = port;
    cout << "Job.cpp: 42" << endl;
    pthread_t p;
    cout << "Job.cpp: 43" << endl;
    listen(server_fd, 3);
    cout << "Job.cpp: 44" << endl;
    arguments* args = new arguments;
    cout << "Job.cpp: 45" << endl;
    args->soc = server_fd;
    cout << "Job.cpp: 46" << endl;
    args->localPath = PathToLocalFile;
    cout << "Job.cpp: 47" << endl;
    pthread_create(&p, NULL, acceptingForAllowPullJob, (void*) args);
    cout << "Job.cpp: 48" << endl;
}

void Job::PullJob(string path) {
    cout << "Job.cpp: Starting to pull the job." << endl;
    PathToLocalFile = path;
    cout << "Job.cpp: 49" << endl;
    int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
    cout << "Job.cpp: 50" << endl;
    struct sockaddr_in serv_addr;
    cout << "Job.cpp: 51" << endl;
    serv_addr.sin_family = AF_INET;
    cout << "Job.cpp: 52" << endl;
    serv_addr.sin_port = htons(binaryPort);
    cout << "Job.cpp: 53" << endl;
    inet_pton(AF_INET, binaryIp, &serv_addr.sin_addr);
    cout << "Job.cpp: 54" << endl;
    connect(soc_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    cout << "Job.cpp: 55" << endl;
    char buff[2048];
    cout << "Job.cpp: 56" << endl;
    int fd = open((char *)&path[0], O_WRONLY | O_CREAT | O_TRUNC, 0777);
    cout << "Job.cpp:  The fd opened is " << fd << endl;
    if (fd < 0) {
        perror("Job.cpp:  PullJob file creation failed: ");
        exit(1);
    }
    cout << "Job.cpp: 57" << endl;
    while(true){
        cout << "Job.cpp: 58" << endl;
        int r = read(soc_fd, buff, 2048);
        cout << "Job.cpp: 59" << endl;
        if (!r){
            cout << "Job.cpp: 60" << endl;
            break;
        }
        cout << "Job.cpp: 61" << endl;
        int s =0;
        while(s<r){
            s += write(fd, buff+s, r-s);
        }
    }
    close(fd);
    close(soc_fd);
    cout << "Job.cpp: Finished pulling the job." << endl;
}

SharedFiles & Job::GetStorage() {
    return shared_files;
}

void Job::Run() {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t r;
    fp = fopen((char*) &PathToLocalFile[0], "r");
    cout << "Job.cpp: Opened the binary file." << endl;

    map<string, int> files = map<string, int>();
    while ((r = getline(&line, &len, fp)) != -1) {
      cout << "Job.cpp: The value of r is " + to_string(r) + ". Starting the if statement to check for newline char." << endl;
        if(line[r-1] == '\n'){line[r-1] = 0;}
	cout << "Job.cpp: The value of r is " + to_string(r) + ".  Ending the if statement to check for newline char." << endl;
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
    cout << "Job.cpp: Finished the job." << endl;
}

int Job::SerializeSelf(char *buff) {
    cout << "Job.cpp: Starting to serialize the job." << endl;
    serializedJob * s = (serializedJob*)buff;
    int ps = 4+4+15;
    s->required_compute = htonl(required_compute);
    s->binaryPort = htonl(binaryPort);
    strncpy(s->binaryIp, binaryIp,15);
    ps += shared_files.serializeSelf((char *) &s->sf);
    s->packageSize = htonl(ps);
    cout << "Job.cpp:  The serialized version of the job is about to be returned." << endl;
    return ps + 4;
}

Job::Job(char *serializedVersion) {
    serializedJob * s = (serializedJob*)serializedVersion;
    required_compute = ntohl(s->required_compute);
    binaryPort = ntohl(s->binaryPort);
    strncpy(binaryIp, s->binaryIp,15);
    shared_files = SharedFiles((char*)&s->sf);
}
