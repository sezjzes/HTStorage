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
    /**
    int longitude;
    int latitude;
    int region;
     */
public:
    // Checks if the resource_ad_type needs to be updated or not based on the changes in storage or compute.
    void updateResourceAdType();
}
#endif //HTSTORAGE_RESOURCE_AD_H
