//
// Created by Josh Szczesniak on 3/14/21.
//

#ifndef HTSTORAGE_CLIENT_H
#define HTSTORAGE_CLIENT_H
#include "Job.h"
#include "SharedFiles.h"

class Client {
    private:
        Job j;
    public:
        /**
         * create a client and to create and send a job
         */
        Client(string binary, int compute, string files, int writeSize);
       ~Client();

        /**
         * send a job to the manager
         */
        void sendJobToManger(char* ip, int port);

        void waitForComplete();
};


#endif //HTSTORAGE_CLIENT_H
