#include "Manager.h"
#include "Job.h"
#include <queue>
#include <vector>
#include <unordered_set>
#include <unordered_map>

queue<Job> Manager::getQueue() {
    return job_queue;
}

void Manager::addToQueue(Job job) {
    job_queue.push(job);
}

Job Manager::getNextJob() {
    return job_queue.pop();
}

// TODO: Need to figure out how fault tolerance plays into this list, how it gets removed. Perhaps has to check a deleted node list before adding it. or can check for a key in the resource_map first before moving forward.
// Match compute first then closest storage.
vector<int> Manager::MatchJobToResources1(Job job) {
    int required_compute = job.getRequiredCompute();
    int required_storage = job.getRequiredStorage();
    vector<int> resource_vector;

    // Finds the available compute and adds it to the resource_vector as the first element.
    for (auto i = resource_map.begin(); i != resource_map.end(); ++i) {
        int available_compute = i->second.getAvailableCompute();
        if (required_compute > 0 && available_compute >= required_compute) {
            resource_vector.push_back(i->second.getResourceID());
	    break;
        }
    }

    // Finds a primary storage node and a backup storage node. Adds them to the resource_vector.
    map<float, unordered_set<int>> temp_closest_resources = resource_vector.at(0)["storage"];
    for (auto i = temp_closest_resources.begin(); i != temp_closest_resources.end(); ++i) {
        if (resource_vector.size() >= 3) {
            break;
        }
        for (auto j = i->second.begin(); j != i->second.end(); ++j) {
            ResourceAd resource_ad = resource_map.at(*j);
            if (resource_ad.getAvailableStorage() >= required_storage) {
                resource_vector.push_back(*j);
                if (resource_vector.size() >= 3) {
                    break;
                }
            }
        }
    }
    return resource_vector;

}

// Matching storage first then closest compute.
vector<int> Manager::MatchJobToResources2(Job job) {
    int required_storage = job.getRequiredStorage();
    int required_compute = job.getRequiredCompute();
    vector<int> resource_vector;

    // Finds the available primary storage and adds it to the resource_vector as the first element.
    for (auto i = resource_map.begin(); i != resource_map.end(); ++i) {
        int available_storage = i->second.getAvailableStorage();
        if (required_storage > 0 && available_storage >= required_storage) {
            resource_vector.push_back(i->second.getResourceID());
	    break;
        }
    }

    // Finds a backup storage node and adds it to the resource_vector.
    map<float, unordered_set<int>> temp_closest_resources = resource_vector.at(0)["storage"];
    for (auto i = temp_closest_resources.begin(); i != temp_closest_resources.end(); ++i) {
        if (resource_vector.size() >= 2) {
            break;
        }
        for (auto j = i->second.begin(); j != i->second.end(); ++j) {
            ResourceAd resource_ad = resource_map.at(*j);
            if (resource_ad.getAvailableStorage() >= required_storage) {
                resource_vector.push_back(*j);
                if (resource_vector.size() >= 2) {
                    break;
                }
            }
        }
    }

    // Finds the closest Execution node near the primary Storage node and adds
    // it to the front of the resource_vector.
    map<float, unordered_set<int>> closest_compute = resource_vector.at(0)["compute"];
    for (auto i = closest_compute.begin(); i != closest_compute.end(); ++i) {
        if (resource_vector.size() >= 2) {
            break;
        }
        for (auto j = i->second.begin(); j != i->second.end(); ++j) {
            ResourceAd resource_ad = resource_map.at(*j);
            if (resource_ad.getAvailableCompute() >= required_compute) {
                resource_vector.emplace(0, *j);
                break;
            }
        }
    }
    return resource_vector;
}
