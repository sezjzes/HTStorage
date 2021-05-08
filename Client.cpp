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
    cout << "Client.cpp: 1" << endl;
    int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
    cout << "Client.cpp: 2" << endl;
    struct sockaddr_in serv_addr;
    cout << "Client.cpp: 3" << endl;
    serv_addr.sin_family = AF_INET;
    cout << "Client.cpp: 4" << endl;
    serv_addr.sin_port = htons(port);
    cout << "Client.cpp: 5" << endl;
    inet_pton(AF_INET, ip, &serv_addr.sin_addr);
    cout << "Client.cpp: 6" << endl;
    connect(soc_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    cout << "Client.cpp: 7" << endl;
    char buff[2048];
    write(soc_fd, buff, j.SerializeSelf(buff));
    cout << "Client.cpp: 8" << endl;
}

void Client::waitForComplete() {
    cout << "Client.cpp: 9" << endl;
    while(!j.GetStorage().complete){}
    cout << "Client.cpp: 10" << endl;
}
