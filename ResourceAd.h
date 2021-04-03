#ifndef HTSTORAGE_RESOURCE_AD_H
#define HTSTORAGE_RESOURCE_AD_H
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
using namespace std;


class ResourceAd {
private:
    int resource_id;
    string resource_ad_type;
    int available_storage;
    int available_compute;
    int total_storage;
    int total_compute;
    char ip[15];
    int port;

public:
    ResourceAd();
    ResourceAd(int resource_id, string resource_ad_type, int available_storage, int available_compute,
               int total_storage, int total_compute, char ip[15], int port);
    int getResourceID();
    void setResourceID(int resource_id);
    string getResourceAdType();
    void setResourceAdType(string resource_ad_type);
    int getAvailableStorage();
    void setAvailableStorage(int available_storage);
    int getAvailableCompute();
    void setAvailableCompute(int available_compute);
    int getTotalStorage();
    void setTotalStorage(int total_storage);
    int getTotalCompute();
    void setIp(char* ip);
    const char *getIp() const;
    int getPort() const;
    void setPort(int port);

    void setTotalCompute(int total_compute);
    void serializeSelf(char* buff);
    ResourceAd(char* buff);

};

struct serializedResourceAd{
    int resource_id;
    char resource_ad_type[20];
    int available_storage;
    int available_compute;
    int total_storage;
    int total_compute;
    char ip[15];
    int port;
};

static const int resourceAdSize = sizeof(serializedResourceAd);
#endif //HTSTORAGE_RESOURCE_AD_H
