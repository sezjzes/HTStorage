#ifndef HTSTORAGE_EXECUTION_H
#define HTSTORAGE_EXECUTION_H
#include <fstream>
#include "Job.h"
using namespace std;


class Execution {
 private:
    Job job;
    fstream  input_file;
    fstream output_file;
 public:
    Execution();
    ~Execution();
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
