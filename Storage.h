#ifndef HTSTORAGE_STORAGE_H
#define HTSTORAGE_STORAGE_H
#include <string>
#include <set>
#include "SharedFiles.h"
#include "ResourceAd.h"
#include "Resource.h"
using namespace std;


class Storage: public Resource {
private:
    char localIp[15];
    char localPort;
public:
     void allowRecieveStorage();
     void createResourceAd(int space);






    bool checkIfHasFile(std::string file_name);
    void pullInFiles(SharedFiles & shared_files);
    void exportOutputFiles();
    int getSizeOfSharedFiles();
    int getAvailableCapacity();
    int getMaximumCapacity();
};

#endif //HTSTORAGE_STORAGE_H
