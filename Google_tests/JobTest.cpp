//
// Created by Josh Szczesniak on 4/3/21.
//
#include "gtest/gtest.h"
#include "../Job.h"
#include "../SharedFiles.h"
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <filesystem>
TEST(TestJob, allowPullJobAndPullJob){
    //todo: portablize this test
    SharedFiles sf;
    Job j = Job(sf, "/Users/joshszczesniak/Desktop/525/HTStorage/Google_tests/testfiles/testfile.txt", 2048);
    j.AllowPullJob();
    std::__fs::filesystem::remove_all("/Users/joshszczesniak/Desktop/525/HTStorage/Google_tests/testfiles/1");
    mkdir("/Users/joshszczesniak/Desktop/525/HTStorage/Google_tests/testfiles/1", 0777);
    j.PullJob("/Users/joshszczesniak/Desktop/525/HTStorage/Google_tests/testfiles/1/testfile.txt");
    sleep(1);
    int fd2 = open("/Users/joshszczesniak/Desktop/525/HTStorage/Google_tests/testfiles/1/testfile.txt", O_RDONLY, NULL);
    ASSERT_GT(fd2, 0);
    char test123[8];
    test123[7] = 0;
    read(fd2, test123, 7);
    ASSERT_STREQ(test123, "test123");
}

TEST(TestJob, runJob){
    SharedFiles sf = SharedFiles("/Users/joshszczesniak/Desktop/525/HTStorage/Google_tests/testfiles", 2048);
    Job j = Job(sf, "/Users/joshszczesniak/Desktop/525/HTStorage/Google_tests/testfiles/simpleRun.txt", 2048);
    j.Run();
}