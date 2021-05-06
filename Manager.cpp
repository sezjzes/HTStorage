#include "Manager.h"
#include "Job.h"
#include "pinger.h"
#include <queue>
#include <vector>
#include <unordered_set>
#include <unordered_map>
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
#include <cstring>

queue<Job> Manager::getQueue() {
    return job_queue;
}

void Manager::addToQueue(Job job) {
    job_queue.push(job);
}

Job Manager::getNextJob() {
    Job f = job_queue.front();
    job_queue.pop();
    return f;
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
    map<int, unordered_set<int>> temp_closest_resources = closest_resources[resource_vector.at(0)]["storage"];
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
    map<int, unordered_set<int>> temp_closest_resources = closest_resources[resource_vector.at(0)]["storage"];
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
    map<int, unordered_set<int>> closest_compute = closest_resources[resource_vector.at(0)]["compute"];
    for (auto i = closest_compute.begin(); i != closest_compute.end(); ++i) {
        if (resource_vector.size() >= 2) {
            break;
        }
        for (auto j = i->second.begin(); j != i->second.end(); ++j) {
            ResourceAd resource_ad = resource_map.at(*j);
            if (resource_ad.getAvailableCompute() >= required_compute) {
                resource_vector.push_back(*j);
                resource_vector.reserve(3);
                break;
            }
        }
    }
    return resource_vector;
}


// Matching them at the same time without regards to location.
vector<int> Manager::MatchJobToResources3(Job job) {
    int required_compute = job.getRequiredCompute();
    int required_storage = job.getRequiredStorage();
    vector<int> resource_vector;
    // Finds the available compute and adds it to the resource_vector as the first element.
    for (auto i = resource_map.begin(); i != resource_map.end(); ++i) {
        if (resource_vector.size() >= 3) {
            return resource_vector;
        }
        int available_compute = i->second.getAvailableCompute();
        int available_storage = i->second.getAvailableStorage();
        if (required_compute > 0 && available_compute >= required_compute) {
            resource_vector.emplace(resource_vector.begin(), i->second.getResourceID());
        }
        if (required_storage > 0 && available_storage >= required_storage) {
            resource_vector.push_back(i->second.getResourceID());
        }
    }
    return resource_vector;
}

// Compares the outputs of Algorithms 1 and 2 and selects the optimal output
// between the two available options.
vector<int> Manager::MatchJobToResources4(Job job) {
    vector<int> algorithm_1_resources = MatchJobToResources1(job);
    vector<int> algorithm_2_resources = MatchJobToResources2(job);
    if (algorithm_1_resources.size() < 2) {
        return algorithm_2_resources;
    } else if (algorithm_2_resources.size() < 2) {
        return algorithm_1_resources;
    } else {
        int ping_time_1 = getPingTime(algorithm_1_resources.at(0), algorithm_1_resources.at(1));
        int ping_time_2 = getPingTime(algorithm_2_resources.at(0), algorithm_2_resources.at(1));
        if (ping_time_1 < ping_time_2) {
            return algorithm_1_resources;
        } else if (ping_time_2 < ping_time_1) {
            return algorithm_2_resources;
        } else {
            return algorithm_1_resources;
        }
    }
}


int Manager::getPingTime(int compute_resource, int storage_resource) {
    map<int, unordered_set<int>> temp_closest_resources = closest_resources[compute_resource]["storage"];
    for (auto i = temp_closest_resources.begin(); i != temp_closest_resources.end(); ++i) {
        unordered_set<int>::const_iterator found = i->second.find(storage_resource);
        if (found == i->second.end()) {
            continue;
        } else {
            return *found;
        }
    }
    // Return an arbitrary high number when the ping time is not found.
    return 1000000;
}

void Manager::processIncomingResourceAd(int socFd){
    char buff[resourceAdSize];
    read(socFd, buff, resourceAdSize);
    ResourceAd ra = ResourceAd(buff);
    if (ra.getResourceID() == -1){
        //add new resource
        ra.setResourceID(nextId++);
        resourceListEntry rl[resource_map.size()];
        int j = 0;
        for (auto i = resource_map.begin(); i != resource_map.end(); ++i) {
            strncpy(rl[j].ip, i->second.getIp(), 15);
            rl[j].resourceId = i->first;

            j++;
        }
        //send resourceId;
        int nrid = htonl(ra.getResourceID());
        write(socFd, (char*) &nrid, 4);
        //send size of list;
        int nsol = htonl(resource_map.size());
        write(socFd, (char*) &nsol, 4);
        //send message
        write(socFd, (char*) rl, resource_map.size() * sizeof(resourceListEntry));

        read(socFd, (char*) rl, resource_map.size() * sizeof(resourceListEntry));

        closest_resources[ra.getResourceID()] = unordered_map<string, map<int, unordered_set<int>>>();
        closest_resources[ra.getResourceID()]["storage"] = map<int, unordered_set<int>>();
        closest_resources[ra.getResourceID()]["compute"] = map<int, unordered_set<int>>();
        for(int i = 0; i < resource_map.size(); i++){
            string type = resource_map[rl[i].resourceId].getResourceAdType();
            auto &m = closest_resources[ra.getResourceID()][type];
            if(m.find(rl[i].pingTime) == m.end()){
                m[rl[i].pingTime] = unordered_set<int>();
                m[rl[i].pingTime].insert(rl[i].resourceId);
            } else {
                m[rl[i].pingTime].insert(rl[i].resourceId);
            }
            auto &m2 = closest_resources[rl[i].resourceId][ra.getResourceAdType()];
            if(m2.find(rl[i].pingTime) == m.end()){
                m2[rl[i].pingTime] = unordered_set<int>();
                m2[rl[i].pingTime].insert(ra.getResourceID());
            } else {
                m2[rl[i].pingTime].insert(ra.getResourceID());
            }
        }
        resource_map[ra.getResourceID()] = ra;
    } else {
        //update resource
        resource_map[ra.getResourceID()] = ra;
    }
}

static void* acceptNewResourceAdsLoop(void * args) {
    argumentsManager *arg = (argumentsManager *) args;
    Manager &m = *arg->m;
    int soc_fd, server_fd;
    server_fd = arg->soc;
    struct sockaddr address;
    int addrlen = sizeof(address);

    while (true) {
        soc_fd = accept(server_fd, &address,
                        (socklen_t *) &addrlen);
        if (soc_fd < 0) {
            perror("accept");
            break;
        }

        m.processIncomingResourceAd(soc_fd);
    }
}

void Manager::acceptNewResourceAds() {
    pinger pi= pinger();
    cout << "ip address to connect to: " << pi.getLocalIp() << endl;
    int server_fd;
    int port;
    int opt = 1;
    struct sockaddr_in address;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
               &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    for (port = 8080; 1; ++port) {
        address.sin_port = htons(port);
        if (::bind(server_fd, (struct sockaddr *) &address, (socklen_t)sizeof(address)) == 0){
            break;
        }
    }
    listen(server_fd, 3);
    pthread_t p;
    argumentsManager * args = new argumentsManager();
    args->m = this;
    args->soc = server_fd;
    pthread_create(&p, NULL,
                   acceptNewResourceAdsLoop, (void*)args);
    cout << "accepting new resources on port: " << port << endl;

}

static void* acceptNewJobsLoop(void * args) {
    argumentsManager *arg = (argumentsManager *) args;
    Manager &m = *arg->m;
    int soc_fd, server_fd;
    server_fd = arg->soc;
    struct sockaddr address;
    int addrlen = sizeof(address);

    while (true) {
        soc_fd = accept(server_fd, &address,
                        (socklen_t *) &addrlen);
        if (soc_fd < 0) {
            perror("accept");
            break;
        }
        serializedJob j;
        read(soc_fd, (char*)&j, 4);
        read(soc_fd, (char*)&j+4, ntohl(j.packageSize));
        m.addToQueue(Job((char *) &j));
    }
}

void Manager::acceptNewJobs() {
    int server_fd;
    int port;
    int opt = 1;
    struct sockaddr_in address;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
               &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    for (port = 8080; 1; ++port) {
        address.sin_port = htons(port);
        if (::bind(server_fd, (struct sockaddr *) &address, (socklen_t)sizeof(address)) == 0){
            break;
        }
    }
    listen(server_fd, 3);
    pthread_t p;
    argumentsManager * args = new argumentsManager();
    args->m = this;
    args->soc = server_fd;
    pthread_create(&p, NULL,
                   acceptNewJobsLoop, (void*)args);
    cout << "accepting new jobs on port: " << port << endl;


}
void Manager::sendJobToCompute(Job j, ResourceAd c){
    c.setAvailableCompute(c.getAvailableCompute() - j.getRequiredCompute());
    int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(c.getPort());
    inet_pton(AF_INET, c.getIp(), &serv_addr.sin_addr);

    if(connect(soc_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))){
        perror("connect");
    }
    char buff[2048];
    write(soc_fd, buff, j.SerializeSelf(buff));
    cout << "Manager.cpp:  Sent the job to the Execution node." << endl;

}


void Manager::sendStorageToStorage(Job j, ResourceAd c) {
    c.setAvailableCompute(c.getAvailableStorage() - j.getRequiredStorage());
    int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(c.getPort());
    inet_pton(AF_INET, c.getIp(), &serv_addr.sin_addr);
    if(connect(soc_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))){
        perror("connect");
    }
    char buff[2048];
    write(soc_fd, buff, j.GetStorage().serializeSelf(buff));
    cout << "Manager.cpp: Sent the storage to the Storage node." << endl;
}

void Manager::assignJobs() {
    while(true){
        if (job_queue.size() < 1){
            continue;
        }
        Job j = getNextJob();
        cout << "Manager.cpp: Pulled the next Job." << endl;

        cout << "Manager.cpp: Running the current algorithm on the job." << endl;
        vector<int> r = MatchJobToResources1(j);
        cout << "Manager.cpp: Found the resources." << endl;
        // TODO: the expected resource vector size is 3, so why would we repeat the algorithm here?
        while (r.size() < 3){
            vector<int> r = MatchJobToResources1(j);
        }
        ResourceAd &s11 = resource_map[r[1]];
        sendStorageToStorage(j, s11);
        cout << "Manager.cpp:  Sent the storage to the primary Storage node." << endl;
        ResourceAd &s21 = resource_map[r[2]];
        sendStorageToStorage(j, s21);
        cout << "Manager.cpp:  Sent the storage to the backup Storage node." << endl;
        ResourceAd &c1 = resource_map[r[0]];
        sendJobToCompute(j, c1);
        cout << "Manager.cpp:  Sent the Job to the Execution node." << endl;
    }
}
