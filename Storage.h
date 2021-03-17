#ifndef HTSTORAGE_STORAGE_H
#define HTSTORAGE_STORAGE_H
#include <string>
#include <set>
#include "SharedFiles.h"


class Storage {
private:
    SharedFiles shared_files;
public:
    File getFile(std::string file_name);
    bool checkIfHasFile(std::string file_name);
    void pullInFiles(SharedFiles & shared_files);
    void exportOutputFiles();
    int getSizeOfSharedFiles();
    int getAvailableCapacity();
    int getMaximumCapacity();
}

#endif //HTSTORAGE_STORAGE_H
