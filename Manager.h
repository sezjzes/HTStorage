#ifndef HTSTORAGE_MANAGER_H
#define HTSTORAGE_MANAGER_H
#include <queue>
#include "Job.h"

class Manager {
private:
    std::queue<Job> job_queue;
public:
    std::queue<Job> getQueue();
    void addToQueue(Job job);
    Job getNextJob();
    void sortQueue();

}

#endif //HTSTORAGE_MANAGER_H
