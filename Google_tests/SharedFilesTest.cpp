//
// Created by Josh Szczesniak on 3/28/21.
//
#include "gtest/gtest.h"
#include "../SharedFiles.h"
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <filesystem>
TEST(TestSharedFiles, testInitialize){
    SharedFiles sf = SharedFiles("/home/ubuntu/HTStorage/Google_tests/testfiles", 2048);
}

TEST(TestSharedFiles, testbeginSharingFiles){
    SharedFiles sf = SharedFiles("/home/ubuntu/HTStorage/Google_tests/testfiles", 2048);
    //sf.beginSharingFiles(false);
}

TEST(TestSharedFiles, testOpenFileReadOnlyAndAllowReads){
    //todo: portablize this test
    SharedFiles sf = SharedFiles("/home/ubuntu/HTStorage/Google_tests/testfiles", 2048);
    //sf.beginSharingFiles(false);
    int fd = sf.openFileReadOnly("testfile.txt");
    char test123[8];
    test123[7] = 0;
    read(fd, test123, 7);
    ASSERT_STREQ(test123, "test123");
}

TEST(TestSharedFiles, testOpenFileForWriteAndAllowWrites){
    //todo: portablize this test
    SharedFiles sf = SharedFiles("/home/ubuntu/HTStorage/Google_tests/testfiles", 2048);
    //sf.beginSharingFiles(false);
    int fd = sf.openFileWriteOnly("testfile2.txt");
    write(fd, "test123", 7);
    int fd2 = open("/home/ubuntu/HTStorage/Google_tests/testfiles/testfile2.txt", O_RDONLY, NULL);
    char test123[8];
    test123[7] = 0;
    close(fd);
    sleep(1);
    read(fd2, test123, 7);
    ASSERT_STREQ(test123, "test123");
}

TEST(TestSharedFiles, allowPullInFilesAndPullInFiles){
    //todo: portablize this test
    SharedFiles sf = SharedFiles("/home/ubuntu/HTStorage/Google_tests/testfiles", 2048);
    //sf.beginSharingFiles(false);
    std::filesystem::remove_all("/home/ubuntu/HTStorage/Google_tests/testfiles/1");
    mkdir("/home/ubuntu/HTStorage/Google_tests/testfiles/1", 0777);
    sf.setLocalPath("/home/ubuntu/HTStorage/Google_tests/testfiles/1");
    sf.pullInFiles();
    sleep(1);
    int fd2 = open("/home/ubuntu/HTStorage/Google_tests/testfiles/1/testfile.txt", O_RDONLY, NULL);
    ASSERT_GT(fd2, 0);
    char test123[8];
    test123[7] = 0;
    read(fd2, test123, 7);
    ASSERT_STREQ(test123, "test123");
}

TEST(TestSharedFiles, allowWriteOutFilesAndWriteOutFiles){
    //todo: portablize this test
    mkdir("/home/ubuntu/HTStorage/Google_tests/testfiles/2", 0777);
    std::filesystem::remove_all("/home/ubuntu/HTStorage/Google_tests/testfiles/2");
    mkdir("/home/ubuntu/HTStorage/Google_tests/testfiles/2", 0777);
    SharedFiles sf = SharedFiles("/home/ubuntu/HTStorage/Google_tests/testfiles/2", 2048);
    //sf.beginSharingFiles(false);

    sf.setLocalPath("/home/ubuntu/HTStorage/Google_tests/testfiles");
    sf.writeOutFiles();
    sleep(1);
    int fd2 = open("/home/ubuntu/HTStorage/Google_tests/testfiles/2/testfile.txt", O_RDONLY, NULL);
    ASSERT_GT(fd2, 0);
    char test123[8];
    test123[7] = 0;
    read(fd2, test123, 7);
    ASSERT_STREQ(test123, "test123");
}

TEST(TestSharedFiles, testSync){
    SharedFiles sf = SharedFiles("/home/ubuntu/HTStorage/Google_tests/testfiles", 2048);
    sf.connectToSyncLocations();
    sleep(1);
    sf.beginSharingFiles(false);
}
// anything with ping can only be run as admin
//TEST(TestSharedFiles, serializeAndUnserialize){
//    SharedFiles sf = SharedFiles("/home/ubuntu/HTStorage/Google_tests/testfiles", 2048);
//    sf.beginSharingFiles(false);
//    sf.beginSharingFiles(false);
//    char buff[2048];
//    sf.serializeSelf(buff);
//    SharedFiles rsf = SharedFiles(buff);
//    ASSERT_EQ(sf.getSize(), rsf.getSize());
//}
