//
// Created by Josh Szczesniak on 3/28/21.
//
#include "gtest/gtest.h"
#include "../SharedFiles.h"
#include <string>

#include <sys/stat.h>
#include <fcntl.h>
TEST(TestSharedFiles, testInitialize){
    SharedFiles sf = SharedFiles("localpath", 2048);
}

TEST(TestSharedFiles, testbeginSharingFiles){
    SharedFiles sf = SharedFiles("localpath", 2048);
    sf.beginSharingFiles();
}

TEST(TestSharedFiles, testOpenFileReadOnlyAndAllowReads){
    //todo: portablize this test
    SharedFiles sf = SharedFiles("/Users/joshszczesniak/Desktop/525/HTStorage/Google_tests", 2048);
    sf.beginSharingFiles();
    int fd = sf.openFileReadOnly("testfile.txt");
    char test123[8];
    test123[7] = 0;
    read(fd, test123, 7);
    ASSERT_STREQ(test123, "test123");
}

TEST(TestSharedFiles, testOpenFileForWriteAndAllowWrites){
    //todo: portablize this test
    SharedFiles sf = SharedFiles("/Users/joshszczesniak/Desktop/525/HTStorage/Google_tests", 2048);
    sf.beginSharingFiles();
    int fd = sf.openFileWriteOnly("testfile2.txt");
    write(fd, "test123", 7);
    int fd2 = open("/Users/joshszczesniak/Desktop/525/HTStorage/Google_tests/testfile2.txt", O_RDONLY, NULL);
    char test123[8];
    test123[7] = 0;
    close(fd);
    sleep(1);
    read(fd2, test123, 7);
    ASSERT_STREQ(test123, "test123");
}

