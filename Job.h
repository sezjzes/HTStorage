//
// Created by Josh Szczesniak on 3/13/21.
//

#ifndef HTSTORAGE_JOB_H
#define HTSTORAGE_JOB_H
#include <fstream>
#include "SharedFiles.h"

class Job {
private:
    ofstream code;
    SharedFiles shared_files;
    int sub_jobs_left_to_run;
    int total_number_of_sub_jobs;
    double percent_completed;
    double current_runtime;
    double max_runtime;
public:
    //client

    /**
     * create a job object from the client
     * @param files associated files object
     * @param code code to be run (may not be best as a char* unsure what to do)
     */
    Job(SharedFiles files, ofstream code);
    ~Job();

    // Update the current runtime of the job.
    void UpdateRuntime(double completed_partial_job_runtime);

    void UpdatePercentCompleted();

    void UpdateNumberOfRemainingJobs();
    /**
     * set up a way for job to be requested by storage
     */
    void AllowPullJob();
    //manager

    /**
     * get the estimated time of the job
     * @return the estimated time in ms of the job
     */
    int GetTime();
    //execution
    /**
     * run the job on the server
     */
    void Run();

    /**
     * get this job's info from client
     */
    void PullJob();

    /**
     * get the attached Shared_files object
     */
    SharedFiles GetStorage();

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
