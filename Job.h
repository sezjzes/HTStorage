//
// Created by Josh Szczesniak on 3/13/21.
//

#ifndef HTSTORAGE_JOB_H
#define HTSTORAGE_JOB_H
#include <fstream>
#include "SharedFiles.h"

class Job {
public:
    char binaryIp[15];
    int binaryPort;
    int required_compute;
    int required_storage;
    string path_to_binary;
    //client

    /**
     * create a job object from the client
     * @param files associated files object
     * @param code code to be run (may not be best as a char* unsure what to do)
     */
    Job(int compute, int storage, string path_to_binary);
    Job();
    ~Job();
    /**
     * set up a way for job to be requested by storage
     */
    void AllowPullJob();
    //manager

    int getRequiredStorage();

    int getRequiredCompute();
    //execution
    /**
     * run the job on the server
     */
    void Run();

    /**
     * get this job's info from client
     * @path where to put job
     */
    void PullJob(string path);

    /**
     * get the attached Shared_files object
     */
    SharedFiles & GetStorage();

    //network
    /**
     * create a serialized object for sending over the network
     */
    int SerializeSelf(char *buff);
    /**
     * create a files object to add files to from a serialized object that was sent over the network
     */
    Job(char* serializedVersion);


};
struct __attribute__((__packed__)) serializedJob{
    int packageSize;
    char binaryIp[15];
    int binaryPort;
    int required_compute;
    SharedFiles::serialized sf;
};


#endif //HTSTORAGE_JOB_H
