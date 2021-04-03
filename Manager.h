#ifndef HTSTORAGE_MANAGER_H
#define HTSTORAGE_MANAGER_H
#include <queue>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include "Job.h"
#include "Execution.h"
#include "ResourceAd.h"
using namespace std;
class Manager {
public:
    queue<Job> job_queue;
    unordered_map<int, unordered_map<string, map<int, unordered_set<int>>>> closest_resources;
    unordered_map<int, ResourceAd> resource_map;
    int nextId = 0;
    // TODO: Create an unordered map to the ip address and port of each resource.


    void acceptNewResourceAds();

    void acceptNewJobs();

    void assignJobs();

    void processIncomingResourceAd(int socFd);

    void sendJobToCompute(Job j, ResourceAd c);




    queue<Job> getQueue();
    void addToQueue(Job job);
    Job getNextJob();
    // The returned vector of ints is the list of IDs of resources that can be used for storage.  The first item in the vector will be the one with the executable to run.
    // Matching compute first then closest storage.
    vector<int> MatchJobToResources1(Job job);
    // Matching storage first then closest compute.
    vector<int> MatchJobToResources2(Job job);
    // Matching them at the same time without regards to location.
    vector<int> MatchJobToResources3(Job job);
    // MAYBE match storage and compute at the same time to find optimal balance.  Can run the searches 1 and 2 and find the best overlap.
    vector<int> MatchJobToResources4(Job job);


    // Pings the Resources for updates on their available resources.
    void getResourceAdUpdates();
    // Updates the resource_map's current ResourceAd for the specific resource.
    void updateResourceAd(ResourceAd);

    void sendStorageToStorage(Job job, ResourceAd ad);
};


struct argumentsManager{
    int soc;
    Manager *m;
};

struct __attribute__((__packed__))  resourceListEntry {
    int resourceId;
    char ip[15];
    int pingTime;
};

#endif //HTSTORAGE_MANAGER_H
