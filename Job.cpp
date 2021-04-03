//
// Created by Josh Szczesniak on 3/13/21.
//

#include "Job.h"

Job::Job(SharedFiles files, string path_to_code){
    shared_files = files;
}

Job::~Job() {}


int Job::getRequiredStorage() {
    return shared_files.getSize();
}

int Job::getRequiredCompute() {
    return required_compute;
}

Job::Job() {}
