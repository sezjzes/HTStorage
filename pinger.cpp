//
// Created by Josh Szczesniak on 4/1/21.
//

#include "pinger.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/time.h>

#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/file.h>

#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
using namespace std;

unsigned short checksum(void *b, int len)
{
    cout << "pinger.cpp: 1" << endl;
    unsigned short *buf = (unsigned short *)b;
    unsigned int sum=0;
    unsigned short result;

    cout << "pinger.cpp: 2" << endl;
    for ( sum = 0; len > 1; len -= 2 )
        cout << "pinger.cpp: 3" << endl;
        sum += *buf++;
        cout << "pinger.cpp: 4" << endl;
    if ( len == 1 )
        cout << "pinger.cpp: 5" << endl;
        sum += *(unsigned char*)buf;
        cout << "pinger.cpp: 6" << endl;
    sum = (sum >> 16) + (sum & 0xFFFF);
    cout << "pinger.cpp: 7" << endl;
    sum += (sum >> 16);
    cout << "pinger.cpp: 8" << endl;
    result = ~sum;
    cout << "pinger.cpp: 9" << endl;
    return result;
}

int pinger::ping(char * ip){
    cout << "pinger.cpp: Starting to send out a ping." << endl;
    string command = "ping -c 1 ";
    cout << "pinger.cpp: 11" << endl;
    command += *ip;
    cout << ip << endl;
    if (strncmp(getLocalIp(), ip, 10)){
        return 0;
    }
//    command += " | head -n 2 | sed -n \'s/.*time=//p\' | sed -n \'s/.*[0-9]* ms//p\'";
//    cout << "pinger.cpp: 13" << endl;
//    int ping_time = system(command.c_str());
//    cout << "pinger.cpp: 14" << endl;
//    cout << "pinger.cpp: Received the ping time: " << ping_time << endl;

    return 1;
    // struct sockaddr from;
    // struct protoent *proto;
    // struct sockaddr whereto;
    // struct sockaddr_in *to = (struct sockaddr_in *) &whereto;
    // to->sin_family = AF_INET;
    // to->sin_addr.s_addr = inet_addr(ip);
    // proto = getprotobyname("icmp");
    // int s = socket(AF_INET, SOCK_RAW, proto->p_proto);
    // static u_char outpack[4096] = {0};
    // struct icmp *icp = (struct icmp *) outpack;
    // icp->icmp_type = ICMP_ECHO;
    // icp->icmp_code = 0;
    // icp->icmp_cksum = 0;
    // icp->icmp_seq = 0;
    // icp->icmp_id = ++pnum;
    // int i, cc;
    // cc = 64;
    // struct timeval *tp = (struct timeval *) &outpack[8];
    // u_char *datap = &outpack[8+sizeof(struct timeval)];
    // for( i=8; i<64-8; i++)	/* skip 8 for time */
    //     *datap++ = i;
    // gettimeofday( tp, NULL);

    // icp->icmp_cksum = checksum( icp, cc );
    // sendto( s, outpack, cc, 0, &whereto, sizeof(struct sockaddr) );
    // struct icmp * icp2;

    // char packet[4096];
    // size_t len = sizeof (packet);
    // socklen_t fromlen = sizeof (from);

    // while(true){
    //     recvfrom(s, (void *) packet, len, 0, &from, &fromlen);
    //     struct ip* ipp = (struct ip*) packet;
    //     int hlen = ipp->ip_hl << 2;
    //     icp2 = (struct icmp *)(packet + hlen);
    //     if(icp->icmp_id == icp2->icmp_id){break;}
    // }
    // struct timeval tv, tt;
    // gettimeofday( &tv, NULL);
    // timersub( &tv, tp, &tt);
    // int triptime = tt.tv_sec*1000000+(tt.tv_usec);
    // return  triptime;
}

char *pinger::getLocalIp() {
    cout << "pinger.cpp: 15" << endl;
    //todo: make this not local host
    char host[256];
    char *ip;
    struct hostent *host_entry;
    int hostname;
    cout << "pinger.cpp: 16" << endl;
    hostname = gethostname(host, sizeof(host)); //find the host name
    cout << "pinger.cpp: 17" << endl;
    host_entry = gethostbyname(host); //find host information
    cout << "pinger.cpp: 18" << endl;
    ip = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));
    cout << "pinger.cpp: 19" << endl;
    return ip;
}
