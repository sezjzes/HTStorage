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
    int localPort;
public:

    string storageDir;
    Storage(const string &storageDir);

    void allowRecieveStorage();
    void createResourceAd(int space);

    int folderNum;
};
struct argumentsStorage{
    int soc;
    Storage *s;
};

#endif //HTSTORAGE_STORAGE_H
