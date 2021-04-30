//
// Created by Josh Szczesniak on 4/4/21.
//


#include "SharedFiles.h"
#include <stdio.h>
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
#include <unistd.h>
#include <sys/poll.h>
#include <map>
#include <sys/time.h>
int main() {
    struct timeval tp;
    gettimeofday( &tp, NULL);

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t r;
    fp = fopen((char*) "/Users/joshszczesniak/Desktop/525/HTStorage/tests/binaries/simpleRun.txt", "r");
    string path = "/Users/joshszczesniak/Desktop/525/HTStorage/tests/files/";
    map<string, int> files = map<string, int>();
    while ((r = getline(&line, &len, fp)) != -1) {
        if(line[r-1] == '\n'){line[r-1] = 0;}

        char buff[2048];
        if(!strncmp(line, "compute", 7)){
            int i = atoi(line+8);
            sleep(i);
            continue;
        }
        if(!strncmp(line, "openWrite", 9)){
            string str = line+10;
            files[str] = open((char *)&(path + str)[0], O_WRONLY | O_CREAT | O_TRUNC, 0777);
            continue;
        }
        if(!strncmp(line, "openRead", 8)){
            string str = line+9;
            files[str] = open((char *)&(path + str)[0], O_RDONLY, NULL);
            continue;
        }
        if(!strncmp(line, "write", 5)){
            char *token = strtok(line +6, " ");
            string str = token;
            int a = atoi(strtok(NULL, " "));
            write(files[str], strtok(NULL, " "), a);
            continue;
        }
        if(!strncmp(line, "read", 4)){
            char *token = strtok(line +5, " ");
            string str = token;
            int a = atoi(strtok(NULL, " "));
            read(files[str], buff, a);
            continue;
        }
    }

    fclose(fp);
    struct timeval tv, tt;
    gettimeofday( &tv, NULL);
    timersub( &tv, &tp, &tt);
    int triptime = tt.tv_sec*1000000+(tt.tv_usec);
    std::cout<<triptime/1000000.0<<std::endl;

}
