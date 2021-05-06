//
// Created by Josh Szczesniak on 3/13/21.
//

#ifndef HTSTORAGE_SHARED_FILES_H
#define HTSTORAGE_SHARED_FILES_H

#include <fstream>
#include <iostream>
#include <string>
#include <list>

using namespace std;
struct Location{
    char ip[15];
    bool isClient;
    int readPort;
    int writePort;
    int pullInPort;
    int writeOutPort;
    int completePort;
    int ping;
};




class SharedFiles {
private:
    //where the files are hosted
    int size;
    int syncPort;
    int syncfd;
public:
    pthread_t syncThread;
    list<Location> locations;
    string localPath;
    bool isClient;
    bool complete;
    int getSyncfd() const;

private:
    char syncIP[48];


public:
    //client
    /**
     * create a files object to add files to from the client
     */
    SharedFiles(string pathToSharedFiles, int writeSize);
    SharedFiles();
    ~SharedFiles();
    /**
     * add a file to the file structure
     * @param pathToFile pathToFile on disk
     */

    /**
     * set up a way for files to be requested by storage
     * @return listening socket for pulling in files
     */
    int allowPullInFiles();

    /**
     * set up a way for files to be wrote back from storage
     * @return listening socket for pulling in files
     */
    int allowWriteOutFiles();

    //manager
    /**
     * get the total size of all files needing to be stored
     * @return the size in bytes of all files to be stored
     */
    int getSize();

    //storage
    /**
     * pull all files in from closest copy of the object
     */
    void pullInFiles();

    /**
     * write out all files to all replicas of the object
     */
    void writeOutFiles();

    /**
     * start sharing files from the currents
     *          allow writes
     *          allow reads
     *          allow pull in
     *          allow write out
     *          gets ip
     *          adds it's self to the local locations
     *          some how sends out this location to replicas
     *
     */
    void beginSharingFiles(bool isClient);


    /**
     * gets the local ip
     * @return local ip address
     */
     char* getLocalIp();

    /**
     * allow individual files to be requested for read
     *
     * set up a server socket that spawns a thread with each connection
     * this thread grabs a file name from its first message
     * opens it and keeps the pipe full with that file till it's all read or the connection is closed
     * cleans itself up when the connection is closed or when the entire file is read
     *
     * @return the port of the listening socket
     *
     */
    int allowRead();

    /**
     * allow individual files to be written to
     *
     * set up a server socket that spawns a thread with each connection
     * this thread grabs a file name from its first message
     * opens it and then waits for incoming messages writing them out to said files
     * cleans itself up when the connection is closed
     *
     * @return the port of the listening socket
     *
     */
    int allowWrite();

    //execution
    //look into making this match c++ standard read write
    /**
     * "open" a non-local file in read only mode
     *
     * open a connection so that reads from the returned socket will
     * read out the same as if a file was being read from the storage device
     *
     * note: sockets will have to use c read syntax as there is no good way to wrap a file dir in the C++ fstreams
     *
     * @param fileName file name on client device
     *
     * @return file descriptor
     */
    int openFileReadOnly(string fileName);
    /**
     * "open" a non-local file in write only mode
     *
     * open a connection so that writes to the returned socket will write out to a file on the storage device
     *
     *
     * note: sockets will have to use c read syntax as there is no good way to wrap a file dir in the C++ fstreams
     *
     * @param fileName file name on client device
     *
     *
     * @return file descriptor
     *
     */
    int openFileWriteOnly(string fileName);

    int allowComplete();
    void sendComplete();

    //network
    struct __attribute__((__packed__))  serializedLocation{
        char ip[15];
        bool isClient;
        int readPort; //(network order)
        int writePort; //(network order)
        int pullInPort; //(network order)
        int writeOutPort; //(network order)
        int completePort; //(network order)
    };

    struct __attribute__((__packed__)) serializedLocationList{
        int packageSize; //(network order)
        int numLocations; //(network order)
        serializedLocation locations[10];
    };

    struct __attribute__((__packed__))  serialized {
        int packageSize; //(network order)
        int size; //(network order)
        int syncPort; //(network order) 0 if none
        char syncIp[15]; //
        serializedLocationList locationList;
    };
    /**
     * int package size (network order)
     * int size (network order)
     * int sync port (network order) 0 if none
     * char
     */

    /**
     * create a serialized object for sending over the network
     * @buff where to put the serialized version
     * @return total bites to send
     */
    int serializeSelf(char *buff);

    /**
     *serialize the locationlist
     * @buff where to put the serialized version
     * @return total bites to send
     */
    int serializeLocationList(char *buff);

    /**
     *serialize a location struct
     * @buff where to put the serialized version
     * @return total bites to send
     */
    int serializeLocation(char *buff, Location l);
    /**
     * create a files object from a serialized object that was sent over the network
     * @serializedVersion a char buff containing the serialized version of the data
     */
    SharedFiles(char* serializedVersion);



    /**
     * needs to be called after unserialization to set the local dir to an empty dir where it should keep the files
     */
    void setLocalPath(string lp);

    /**
     * creats a location object from a serialized location object
     * @param sl serialized location object
     * @return location object
     */
    Location unserializeLocation(serializedLocation *sl);

    /**
     * replaces location list with new serializedLocationList
     * @param sll
     */
    void unserializeLocationList(serializedLocationList *sll);

    /**
     * sorts locations closest to furthest
     */
    void sortLocations();

    /**
     * opens a port that can be connected to
     * reads incoming messages as new locations
     * sends out updates list to all connect locations
     */
    void handelSyncLocations();

    /**
     * connects to sync location based on ip and port
     * sets sync_fd to the socket
     */
    void connectToSyncLocations();

    /**
     * sends a location to sync_fd
     * @param l
     */
    void sendNewLocation(Location l);


    void addLocation(Location l);
};

struct arguments{
    int soc;
    string localPath;
    SharedFiles * sf;
};
#endif //HTSTORAGE_SHARED_FILES_H
