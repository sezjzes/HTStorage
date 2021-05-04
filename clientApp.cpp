//
// Created by Josh Szczesniak on 4/30/21.
//

#include "clientApp.h"
#include <sys/time.h>
#include "pinger.h"
#include "Client.h"
#include <stdio.h>
#include <ns3/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#define PORT 8082


static void* StartApplicationHelper(void* v) {
    cout<<"client"<<endl;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "10.1.1.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
    }
    send(sock , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
//    struct timeval tp;
//    gettimeofday( &tp, NULL);
//    Client c = Client(
//            "/Users/joshszczesniak/Desktop/525/HTStorage/tests/binaries/simpleRun.txt", 2,
//            "/Users/joshszczesniak/Desktop/525/HTStorage/tests/files", 7);
//    char* ip = "193.168.002.215";
//    pinger p = pinger();
//    std::cout << p.getLocalIp() << std::endl;
//    c.sendJobToManger(ip, 8081);
//    c.waitForComplete();
//
//    struct timeval tv, tt;
//    gettimeofday( &tv, NULL);
//    timersub( &tv, &tp, &tt);
//    int triptime = tt.tv_sec*1000000+(tt.tv_usec);
//    std::cout<<triptime/1000000.0<<std::endl;
}
void clientApp::StartApplication(void){
    pthread_t p;
    pthread_create(&p, NULL,
                   StartApplicationHelper, NULL);

}
