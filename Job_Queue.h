#ifndef HTSTORAGE_JOB_QUEUE_H
#define HTSTORAGE_JOB_QUEUE_H
#include <queue>
#include <Job.h>

class JobQueue {
 private:
    queue<Job> job_queue;
 public:
    queue<Job> getQueue();
    void addToQueue(Job job);
    Job getNextJob();



}

#endif //HTSTORAGE_JOB_QUEUE_H
