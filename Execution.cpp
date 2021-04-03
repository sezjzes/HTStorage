#include "Execution.h"
using namespace std;

Execution::Execution() {}

Execution::~Execution() {}

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


void Execution::allowRecieveJob() {
    pinger p = pinger();
    strncpy(localIp, p.getLocalIp(), 15);
    localPort = 1;
}

void Execution::createResourceAd(int compute) {
    resourceAd = ResourceAd(-1, "compute", 0, compute,
                            0, compute, localIp, localPort);
}

void Execution::executeJobs() {

}
