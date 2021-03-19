//
// Created by Josh Szczesniak on 3/13/21.
//

#include "Job.h"

Job::Job(SharedFiles files, ofstream code) {
    ofstream = code;
    shared_files = files;
    percent_completed = 0.0;
    current_runtime = 0.0;
    max_runtime = 50000000.0;
}

Job::~Job() {
    ofstream = nullptr;
    ~shared_files;
}

void Job::UpdateRuntime(double completed_sub_job_runtime) {
    current_runtime += completed_sub_job_runtime;
}

void Job::UpdatePercentCompleted() {
    percent_completed = sub_jobs_left_to_run / total_number_of_sub_jobs;
}
