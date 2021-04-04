//
// Created by Josh Szczesniak on 3/14/21.
//

#include "Client.h"
#include "SharedFiles.h"
#include "Manager.h"
#include "Job.h"
#include "pinger.h"
#include <queue>
#include <vector>
#include <unordered_set>
#include <unordered_map>
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


Client::Client(string binary, int compute, string files, int writeSize) : j(files, writeSize, binary, compute){
    j.AllowPullJob();
}

Client::~Client() {}

void Client::sendJobToManger(char *ip, int port) {
    int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &serv_addr.sin_addr);
    connect(soc_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    char buff[2048];
    write(soc_fd, buff, j.SerializeSelf(buff));
}

void Client::waitForComplete() {
    while(!j.GetStorage().complete){}
}

