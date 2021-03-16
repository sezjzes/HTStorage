//
// Created by Josh Szczesniak on 3/14/21.
//

#ifndef HTSTORAGE_CLEINT_H
#define HTSTORAGE_CLEINT_H
#include "Job.h"
#include "Shared_Files.h"

class Cleint {
    public:
        /**
         * create a client and to create and send a job
         */
        Cleint();

        /**
         * create a shared files object and add files to it
         */
        Shared_Files Initialize_Shared_Files();

        /**
         * create a job with attached files
         * @param sharedFiles attached files
         */
        Job Initialize_Job(Shared_Files sharedFiles);

        /**
         * send a job to the manager
         */
        void send_job(Job jobToSend);

};


#endif //HTSTORAGE_CLEINT_H
