//
// Created by Josh Szczesniak on 3/13/21.
//

#ifndef HTSTORAGE_JOB_H
#define HTSTORAGE_JOB_H
#include "Shared_Files.h"

class Job {
    public:
    //client

        /**
         * create a job object from the client
         * @param files associated files object
         * @param code code to be run (may not be best as a char* unsure what to do)
         */
        Job(Shared_Files files, char * code);

        /**
         * set up a way for job to be requested by storage
         */
        void Allow_pull_job();
    //manager

        /**
         * get the estimated time of the job
         * @return the estimated time in ms of the job
         */
        int Get_time();
    //execution
        /**
         * run the job on the server
         */
        void Run();

        /**
         * get this job's info from client
         */
        void Pull_job();

        /**
         * get the attached Shared_files object
         */
        Shared_Files get_storage();

    //network
        /**
         * create a serialized object for sending over the network
         */
        char* SerializeSelf();
        /**
         * create a files object to add files to from a serialized object that was sent over the network
         */
        Job(char* serializedVersion);


};


#endif //HTSTORAGE_JOB_H
