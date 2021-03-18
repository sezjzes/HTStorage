//
// Created by Josh Szczesniak on 3/13/21.
//

#ifndef HTSTORAGE_SHARED_FILES_H
#define HTSTORAGE_SHARED_FILES_H
#include <filesystem>

class SharedFiles {
private:
    filesystem::directory_entry current_directory;
public:
    //client
    /**
     * create a files object to add files to from the client
     */
    SharedFiles(filesystem::path & path);
    ~SharedFiles();
    /**
     * add a file to the file structure
     * @param pathToFile pathToFile on disk
     */
    void addFile(char pathToFile);
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
     * allow individual files to be requested for read
     */
    void allowRead();
    /**
     * allow individual files to be written
     */
    void allowWrite();

    //execution
    //look into making this match c++ standard read write
    /**
     * read a file from closest non-local object
     * @param fileName file name on client device
     * @return out of read
     */
    char* read(char* fileName);
    /**
     * write to a file from in closest non-local object
     * @param fileName file name on client device
     * @param toWrite string to write (maybe want bit write instead?)
     */
    void write(char* fileName, char* toWrite);

    //network
    /**
     * create a serialized object for sending over the network
     */
    char* serializeSelf();
    /**
     * create a files object to add files to from a serialized object that was sent over the network
     */
    Shared_Files(char* serializedVersion);


};


#endif //HTSTORAGE_SHARED_FILES_H
