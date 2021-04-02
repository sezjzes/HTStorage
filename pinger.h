//
// Created by Josh Szczesniak on 4/1/21.
//

#ifndef HTSTORAGE_PINGER_H
#define HTSTORAGE_PINGER_H


class pinger {
char pnum = 0;
public:
/**
 * pass in an ip get the time it takes to get to that ip
 * @param ip address to ping
 * @return round trip time
 */
int ping(char * ip);
};


#endif //HTSTORAGE_PINGER_H
