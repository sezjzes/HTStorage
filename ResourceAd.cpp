#include "ResourceAd.h"
#include <string>
using namespace std;

int ResourceAd::getResourceID() {
    return resource_id;
}

void ResourceAd::setResourceID(int new_resource_id) {
    resource_id = new_resource_id;
}

string ResourceAd::getResourceAdType() {
    return resource_ad_type;
}

void ResourceAd::setResourceAdType(string new_resource_ad_type) {
    resource_ad_type = new_resource_ad_type;
}

int ResourceAd::getAvailableStorage() {
    return available_storage;
}

void ResourceAd::setAvailableStorage(int new_available_storage) {
    available_storage = new_available_storage;
}

int ResourceAd::getAvailableCompute() {
    return available_compute;
}

void ResourceAd::setAvailableCompute(int new_available_compute) {
    available_compute = new_available_compute;
}

int ResourceAd::getTotalStorage() {
    return total_storage;
}

void ResourceAd::setTotalStorage(int new_total_storage) {
    total_storage = new_total_storage;
}

int ResourceAd::getTotalCompute() {
    return total_compute;
}

void ResourceAd::setTotalCompute(int new_total_compute) {
    total_compute = new_total_compute;
}
