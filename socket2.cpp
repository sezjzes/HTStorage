//
// Created by Josh Szczesniak on 5/5/21.
//

#include "socket2.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-address.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

extern pthread_cond_t s;
static void sendCb( ns3::Ptr<ns3::Socket> soc, uint32_t t){
    pthread_cond_signal(&s);
}
static void recvCb( ns3::Ptr<ns3::Socket> soc){
    pthread_cond_signal(&s);
}

bool socket2::connectionRequest( ns3::Ptr<ns3::Socket> soc, const ns3::Address& addressFrom){
    return true;
}
void socket2::connectionRecieve( ns3::Ptr<ns3::Socket> soc, const ns3::Address& address){
    socNum++;
    allSockets[socNum] = soc;

}


socket2::socket2(const ns3::Ptr<ns3::Node>& n){
    node = n;
    socNum = 0;
    pthread_cond_init(&s, NULL);
    pthread_mutex_init(&m, NULL);

}

int socket2::socket(int domain, int type, int protocol){
    socNum++;
    allSockets[socNum] = ns3::Socket::CreateSocket(node, ns3::TcpSocketFactory::GetTypeId());
    allSockets[socNum]->SetSendCallback(ns3::MakeCallback(sendCb));
    allSockets[socNum]->SetRecvCallback(ns3::MakeCallback(recvCb));
    return socNum;
}

int socket2::accept(int socket, struct sockaddr *address,
           socklen_t *address_len){
    allSockets[socket]->SetAcceptCallback(
            ns3::MakeCallback(&socket2::connectionRequest, this),
            ns3::MakeCallback(&socket2::connectionRecieve, this));
}

int socket2::bind(int socket, const struct sockaddr *address,
         socklen_t address_len){
    return allSockets[socket]->Bind(
            ns3::InetSocketAddress(ns3::Ipv4Address(((sockaddr_in*)address)->sin_addr.s_addr),
                                   ((sockaddr_in*)address)->sin_port));
}

int socket2::connect(int socket, const struct sockaddr *address,
            socklen_t address_len){
    return allSockets[socket]->Connect(
            ns3::InetSocketAddress(ns3::Ipv4Address(((sockaddr_in*)address)->sin_addr.s_addr),
                                   ((sockaddr_in*)address)->sin_port));
}

ssize_t socket2::read(int socket, void *buffer, size_t length){
    while (!allSockets[socket]->GetRxAvailable()){
        pthread_cond_wait(&s, &m);
        pthread_cond_signal(&s);
        pthread_mutex_unlock(&m);
    };
    return allSockets[socket]->Recv((uint8_t *)buffer, length, 0);
}

ssize_t socket2::write(int socket, const void *message, size_t length){
    while (!allSockets[socket]->GetTxAvailable()){
        pthread_cond_wait(&s, &m);
        pthread_cond_signal(&s);
        pthread_mutex_unlock(&m);
    }
    return allSockets[socket]->Send((uint8_t *)message, length, 0);
}

int socket2::listen(int socket, int backlog) {
    return allSockets[socket]->Listen();
}

int socket2::close(int socket) {
    allSockets[socket]->ShutdownRecv();
    allSockets[socket]->ShutdownSend();
    return 0;
}
