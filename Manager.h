#ifndef HTSTORAGE_MANAGER_H
#define HTSTORAGE_MANAGER_H
#include <queue>
#include <vector>
#include <unordered_map>
#include <set>
#include "Job.h"
#include "Execution.h"
#include "ResourceAd.h"
using namespace std;

class Manager {
private:
    std::queue<Job> job_queue;
    unordered_map<int, map<float, set<int>>> closest_resources;
    unordered_map<int, ResourceAd> resource_map;
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
