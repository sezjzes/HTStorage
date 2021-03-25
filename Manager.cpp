#include "Manager.h"
#include "Job.h"
#include <queue>
#include <vector>
#include <set>
#include <unordered_map>

queue<Job> Manager::getQueue() {
    return job_queue;
}

void Manager::addToQueue(Job job) {
    job_queue.push(job); //push(job);
}

Job Manager::getNextJob() {
    return job_queue.pop();
}

// Match compute first then closest storage.
vector<int> Manager::MatchJobToResources1(Job job) {
    int required_compute = job.getRequiredCompute();
    int required_storage = job.getRequiredStorage();
    vector<int> resource_vector;
    set<int> available_storage_resources;

    for (auto i = resource_map.begin(); i != resource_map.end(); ++i) {
        int available_compute = i->second.getAvailableCompute();
	if (required_compute <= 0) {
	  break;
	} else if (required_compute > 0 && available_compute >= required_compute) {
            resource_vector.push_back(i->second.getResourceID());
	    break;
        }
    }



            int available_storage = i->second.getAvailableStorage();
        if (required_storage > 0 && available_storage <= required_storage) {
            available_storage_resources.insert(i->second.getResourceID());
        }





    return resource_vector;

}

// Matching storage first then closest compute.
vector<int> Manager::MatchJobToResources2(Job job) {
    int required_storage = job.getRequiredStorage();
    int required_compute = job.getRequiredCompute();
    vector<int> resource_vector;

    for (auto i = resource_map.begin(); i != resource_map.end(); ++i) {
        int available_compute = i->second.getAvailableCompute();
        if (required_compute > 0 && available_compute <= required_compute) {
            resource_vector.push_back(i->second.getResourceID());
            required_compute -= available_compute;
        } else {}

        int available_storage = i->second.getAvailableStorage();
        if (required_storage > 0 && available_storage <= required_storage) {
            resource_vector.push_back(i->second.getResourceID());
            required_storage -= available_storage;
        }
    }

    return resource_vector;

}
