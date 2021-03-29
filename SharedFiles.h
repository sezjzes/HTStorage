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
    char* ip;
    int readPort;
    int writePort;
};

class SharedFiles {
private:
    //where the files are hosted
    string localPath;
    int size;
    list<Location> locations;


public:
    //client
    /**
     * create a files object to add files to from the client
     */
    SharedFiles(string pathToSharedFiles, int writeSize);

    ~SharedFiles();
    /**
     * add a file to the file structure
     * @param pathToFile pathToFile on disk
     */

    /**
     * set up a way for files to be requested by storage
     */
    void allowPullInFiles();

    /**
     * set up a way for files to be wrote back from storage
     */
    void allowWriteOutFiles();

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
     *          gets ip
     *          adds it's self to the local locations
     *          some how sends out this location to replicas
     *
     */
    void beginSharingFiles();


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

    //network
    /**
     * create a serialized object for sending over the network
     */
    char* serializeSelf();
    /**
     * create a files object to add files to from a serialized object that was sent over the network
     */
    //SharedFiles(char* serializedVersion);


};


#endif //HTSTORAGE_SHARED_FILES_H
