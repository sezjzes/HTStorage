#include "Manager.h"
#include "Job.h"
#include <queue>
#include <vector>
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

vector<int> Manager::MatchJobToResources(Job job) {
    required_storage = job.getRequiredStorage();
    required_compute = job.getRequiredCompute();
    vector<int> resource_vector;

    return resource_vector;

}
