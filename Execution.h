#ifndef HTSTORAGE_EXECUTION_H
#define HTSTORAGE_EXECUTION_H
#include <fstream>
#include "Job.h"
#include "ResourceAd.h"
#include "Resource.h"
using namespace std;


class Execution: public Resource{
 private:
    Job job;
    char localIp[15];
    int localPort;

 public:
    Execution();
    ~Execution();
    void allowRecieveJob();
    void createResourceAd(int compute);
    void executeJobs();







    void runJob();
    void getStorage();
    void read();
    void write();
    Job getJob();
    void setJob(Job & job);
    fstream getFile();
    void setCurrentFile(fstream file);
};

#endif //HTSTORAGE_EXECUTION_H
