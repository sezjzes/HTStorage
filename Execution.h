#ifndef HTSTORAGE_EXECUTION_H
#define HTSTORAGE_EXECUTION_H
#include <fstream>
#include "Job.h"


class Execution {
 private:
    Job job;
    File current_file;
 public:
    Execution();
    ~Execution();
    void runJob();
    void getStorage();
    void read();
    void write();
    Job getJob();
    void setJob(Job & job);
    File getCurrentFile();
    void setCurrentFile(File file);
};

#endif //HTSTORAGE_EXECUTION_H
