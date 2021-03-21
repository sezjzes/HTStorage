//
// Created by Josh Szczesniak on 3/14/21.
//

#ifndef HTSTORAGE_CLIENT_H
#define HTSTORAGE_CLIENT_H
#include "Job.h"
#include "SharedFiles.h"

class Client {
    private:
    public:
        /**
         * create a client and to create and send a job
         */
        Client();
       ~Client();

        /**
         * create a shared files object and add files to it
         */
        SharedFiles InitializeSharedFiles();

        /**
         * create a job with attached files
         * @param sharedFiles attached files
         */
        Job InitializeJob(SharedFiles sharedFiles);

        /**
         * send a job to the manager
         */
        void SendJob(Job jobToSend);
};


#endif //HTSTORAGE_CLIENT_H
