#include "Execution.h"
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
#include <stdio.h>
#include <string.h>
using namespace std;

Execution::Execution(string jobDir) {
    Execution::jobDir = jobDir;
    fileNum = 0;
}

Execution::~Execution() {}





static void* acceptNewJobsLoop(void * args) {
    argumentsExecution *arg = (argumentsExecution *) args;
    Execution *e = arg->e;
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
        serializedJob j;
        read(soc_fd, (char*)&j, 4);
        read(soc_fd, (char*)&j+4, ntohl(j.packageSize));
        Job job = Job((char *) &j);
        job.PullJob(e->jobDir + "/" + to_string(e->fileNum++));
        e->job_queue.push(job);
    }
}

void Execution::allowRecieveJob() {
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
    argumentsExecution * args = new argumentsExecution();
    args->e = this;
    args->soc = server_fd;
    pthread_create(&p, NULL,
                   acceptNewJobsLoop, (void*)args);
    localPort = port;
}

void Execution::createResourceAd(int compute) {
    resourceAd = ResourceAd(-1, "compute", 0, compute,
                            0, compute, localIp, localPort);
}

void Execution::executeJobs() {
    while (true){
        if(job_queue.size() < 1){
            continue;
        }
        Job j = job_queue.front();
        job_queue.pop();
        j.GetStorage().connectToSyncLocations();
        j.Run();
        j.GetStorage().sendComplete();
    }
}
