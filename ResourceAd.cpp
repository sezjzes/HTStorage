#include "ResourceAd.h"
#include <string>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "pinger.h"
#include <unistd.h>
#include "Manager.h"
#include <stdio.h>
#include <string.h>

using namespace std;


ResourceAd::ResourceAd() {}

ResourceAd::ResourceAd(int resource_id, string resource_ad_type, int available_storage, int available_compute,
           int total_storage, int total_compute, char ip[15], int port){
    ResourceAd::resource_id = resource_id;
    ResourceAd::resource_ad_type = resource_ad_type;
    ResourceAd::available_storage = available_storage;
    ResourceAd::available_compute = available_compute;
    ResourceAd::total_storage = total_storage;
    ResourceAd::total_compute = total_compute;
    strncpy(ResourceAd::ip, ip, 15);
    ResourceAd::port = port;

}

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

void ResourceAd::setIp(char *ip) {
    strncpy(this->ip, ip, 15);
}

const char *ResourceAd::getIp() const {
    return ip;
}

int ResourceAd::getPort() const {
    return port;
}

void ResourceAd::setPort(int port) {
    ResourceAd::port = port;
}

void ResourceAd::serializeSelf(char *buff) {
    cout << "ResourceAd.cpp: Beginning to serialize the ResourceAd." << endl;
    serializedResourceAd * sr = (serializedResourceAd *) buff;
    sr->resource_id = htonl(resource_id);
    strcpy(sr->resource_ad_type, resource_ad_type.c_str());
    sr->available_storage = htonl(available_storage);
    sr->available_compute = htonl(available_compute);
    sr->total_storage = htonl(total_storage);
    sr->total_compute = htonl(total_compute);
    strncpy(sr->ip, ip, 15);
    sr->port = htonl(port);
}

ResourceAd::ResourceAd(char *buff) {
    serializedResourceAd * sr = (serializedResourceAd *) buff;
    resource_id = htonl(sr->resource_id);
    resource_ad_type = sr->resource_ad_type;
    available_storage = ntohl(sr->available_storage);
    available_compute = ntohl(sr->available_compute);
    total_storage = ntohl(sr->total_storage);
    total_compute = ntohl(sr->total_compute);
    strncpy(ip, sr->ip, 15);
    port = ntohl(sr->port);
}
