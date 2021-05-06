//
// Created by Josh Szczesniak on 5/5/21.
//

#ifndef NS_3SIMULATION_SOCKET2_H
#define NS_3SIMULATION_SOCKET2_H
#include "ns3/socket-factory.h"
#include "ns3/socket.h"
#include "ns3/tcp-socket-factory.h"
#include <netinet/in.h>
#include <map>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

class socket2 {
    int socNum;
    std::map<int, ns3::Ptr<ns3::Socket>> allSockets;
    ns3::Ptr<ns3::Node> node;
    pthread_mutex_t m;
    socket2(const ns3::Ptr<ns3::Node>&  n);
    void connectionRecieve( ns3::Ptr<ns3::Socket> soc, const ns3::Address& address);
    int socket(int domain, int type, int protocol);
    int bind(int socket, const struct sockaddr *address,
             socklen_t address_len);
    int listen(int socket, int backlog);
    int accept(int socket, struct sockaddr *address,
                   socklen_t *address_len);

    int connect(int socket, const struct sockaddr *address,
                    socklen_t address_len);
    int close(int socket);
    ssize_t read(int socket, void *buffer, size_t length);
    ssize_t write(int socket, const void *message, size_t length);

    bool connectionRequest(ns3::Ptr<ns3::Socket> soc, const ns3::Address& addressFrom);
};


#endif //NS_3SIMULATION_SOCKET2_H
