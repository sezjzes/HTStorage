#ifndef HTSTORAGE_EXECUTION_H
#define HTSTORAGE_EXECUTION_H
#include <fstream>
#include "Job.h"


class Execution {
 private:
    Job job;
    std::fstream  input_file;
    std::fstream output_file;
 public:
    Execution();
    ~Execution();
    void runJob();
    void getStorage();
    void read();
    void write();
    Job getJob();
    void setJob(Job & job);
    std::fstream getCurrentFile();
    void setCurrentFile(std::fstream file);
};

#endif //HTSTORAGE_EXECUTION_H
