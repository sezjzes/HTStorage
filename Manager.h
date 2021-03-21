#ifndef HTSTORAGE_MANAGER_H
#define HTSTORAGE_MANAGER_H
#include <queue>
#include <vector>
#include <unordered_map>
#include "Job.h"
#include "Execution.h"
#include "ResourceAd.h"
using namespace std;

class Manager {
private:
    std::queue<Job> job_queue;
    // Tracks the IDs of all currently known resources and their current Resource availabilities.
    // The latest mapping of Resource IDs to the type of resources they are currently advertising
    unordered_map<int, string> resource_ad_type;
    // The top level map is region. The value is a map where the keys are "both," "compute," and "storage." The value of that would be an unordered_map of resource IDs to Resource Ads that are currently advertising either both compute and storage, only storage, or only compute.
    unordered_map<string, unordered_map<string, unordered_map<int, ResourceAd>>> resource_map;

public:
    std::queue<Job> getQueue();
    void addToQueue(Job job);
    Job getNextJob();
    void sortQueue();
    // The returned vector of ints is the list of IDs of resources that can be used for storage.  The first item in the vector will be the one with the executable to run.
    vector<int> MatchJobToResources(Job job);
    // Pings the Resources for updates on their available resources.
    void getResourceAdUpdates();
    // Updates the resource_map's current ResourceAd for the specific resource.
    void updateResourceAd(ResourceAd);



}

#endif //HTSTORAGE_MANAGER_H
