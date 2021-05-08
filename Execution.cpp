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
    cout << "Execution.cpp: 1" << endl;
    argumentsExecution *arg = (argumentsExecution *) args;
    cout << "Execution.cpp: 2" << endl;
    Execution *e = arg->e;
    cout << "Execution.cpp: 3" << endl;
    int soc_fd, server_fd;
    server_fd = arg->soc;
    cout << "Execution.cpp: 4" << endl;
    struct sockaddr address;
    int addrlen = sizeof(address);
    cout << "Execution.cpp: 5" << endl;

    while (true) {
        cout << "Execution.cpp: 6" << endl;
        soc_fd = accept(server_fd, &address,
                        (socklen_t *) &addrlen);
        cout << "Execution.cpp: 7" << endl;
        if (soc_fd < 0) {
            cout << "Execution.cpp: 8" << endl;
            perror("accept");
            cout << "Execution.cpp: 9" << endl;
            break;
        }
        cout << "Execution.cpp: 10" << endl;
        serializedJob j;
        cout << "Execution.cpp: 11" << endl;
        read(soc_fd, (char*)&j, 4);
        cout << "Execution.cpp: 12" << endl;
        read(soc_fd, (char*)&j+4, ntohl(j.packageSize));
        cout << "Execution.cpp: 13" << endl;
        Job job = Job((char *) &j);
        cout << "Execution.cpp: 14" << endl;
        job.PullJob(e->jobDir + "/" + to_string(e->fileNum++));
        cout << "Execution.cpp: 15" << endl;
        e->job_queue.push(job);
        cout << "Execution.cpp: 16" << endl;
    }
}

void Execution::allowRecieveJob() {
    cout << "Execution.cpp: 17" << endl;
    pinger pi = pinger();
    cout << "Execution.cpp: 18" << endl;
    strncpy(localIp, pi.getLocalIp(), 15);
    cout << "Execution.cpp: 19" << endl;

    int server_fd;
    int port;
    int opt = 1;
    struct sockaddr_in address;
    cout << "Execution.cpp: 20" << endl;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    cout << "Execution.cpp: 21" << endl;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
               &opt, sizeof(opt));
    cout << "Execution.cpp: 22" << endl;
    address.sin_family = AF_INET;
    cout << "Execution.cpp: 23" << endl;
    address.sin_addr.s_addr = INADDR_ANY;
    cout << "Execution.cpp: 24" << endl;
    for (port = 8080; 1; ++port) {
        cout << "Execution.cpp: 25" << endl;
        address.sin_port = htons(port);
        cout << "Execution.cpp: 26" << endl;
        if (::bind(server_fd, (struct sockaddr *) &address, (socklen_t)sizeof(address)) == 0){
            cout << "Execution.cpp: 27" << endl;
            break;
        }
        cout << "Execution.cpp: 28" << endl;
    }
    cout << "Execution.cpp: 29" << endl;
    listen(server_fd, 3);
    cout << "Execution.cpp: 30" << endl;
    pthread_t p;
    cout << "Execution.cpp: 31" << endl;
    argumentsExecution * args = new argumentsExecution();
    cout << "Execution.cpp: 32" << endl;
    args->e = this;
    cout << "Execution.cpp: 33" << endl;
    args->soc = server_fd;
    cout << "Execution.cpp: 34" << endl;
    pthread_create(&p, NULL,
                   acceptNewJobsLoop, (void*)args);
    cout << "Execution.cpp: 35" << endl;
    localPort = port;
    cout << "Execution.cpp: 36" << endl;
}

void Execution::createResourceAd(int compute) {
    cout << "Execution.cpp: 37" << endl;
    resourceAd = ResourceAd(-1, "compute", 0, compute,
                            0, compute, localIp, localPort);
    cout << "Execution.cpp: 38" << endl;
}

void Execution::executeJobs() {
    cout << "Execution.cpp: Starting to execute the jobs." << endl;
    while (true){
        cout << "Execution.cpp: 39" << endl;
        if(job_queue.size() < 1){
            cout << "Execution.cpp: 40" << endl;
            continue;
            cout << "Execution.cpp: 41" << endl;
        }
        cout << "Execution.cpp: 42" << endl;
        Job j = job_queue.front();
        cout << "Execution.cpp: Got the top Job." << endl;
        job_queue.pop();
        cout << "Execution.cpp: 43" << endl;
        j.GetStorage().connectToSyncLocations();
        cout << "Execution.cpp: Got relevant storage." << endl;
        j.Run();
        cout << "Execution.cpp: Ran the job." << endl;
        j.GetStorage().sendComplete();
        cout << "Execution.cpp: Sent the Job Complete message." << endl;
    }
    cout << "Execution.cpp: 44" << endl;
}
