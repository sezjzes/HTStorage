#ifndef HTSTORAGE_EXECUTION_H
#define HTSTORAGE_EXECUTION_H
#include <fstream>
#include "Job.h"
#include "ResourceAd.h"
#include "Resource.h"
#include <queue>
using namespace std;


class Execution: public Resource{
 private:
    Job job;
    char localIp[15];
    int localPort;

public:

    string jobDir;
    int fileNum;
    Execution(string jobDir);
    ~Execution();
    void allowRecieveJob();
    void createResourceAd(int compute);
    void executeJobs();

    queue<Job> job_queue;
};

struct argumentsExecution{
    int soc;
    Execution *e;
};

#endif //HTSTORAGE_EXECUTION_H
