//
// Created by Josh Szczesniak on 3/13/21.
//

#ifndef HTSTORAGE_SHARED_FILES_H
#define HTSTORAGE_SHARED_FILES_H


class Shared_Files {
    public:
    //client
        /**
         * create a files object to add files to from the client
         */
        Shared_Files();
        /**
         * add a file to the file structure
         * @param pathToFile pathToFile on disk
         */
        void Add_file(char pathToFile);
        /**
         * set up a way for files to be requested by storage
         */
        void Allow_pull_in_files();

        /**
         * set up a way for files to be wrote back from storage
         */
        void Allow_write_out_files();

    //manager
        /**
         * get the total size of all files needing to be stored
         * @return the size in bytes of all files to be stored
         */
        int Get_size();

    //storage
        /**
         * pull all files in from closest copy of the object
         */
        void Pull_in_files();
        /**
         * write out all files to all replicas of the object
         */
        void Write_out_files();

        /**
         * allow individual files to be requested for read
         */
        void Allow_read();
        /**
         * allow individual files to be written
         */
        void Allow_write();

    //execution
        //look into making this match c++ standard read write
        /**
         * read a file from closest non-local object
         * @param fileName file name on client device
         * @return out of read
         */
        char* Read(char* fileName);
        /**
         * write to a file from in closest non-local object
         * @param fileName file name on client device
         * @param toWrite string to write (maybe want bit write instead?)
         */
        void Write(char* fileName, char* toWrite);

    //network
        /**
         * create a serialized object for sending over the network
         */
        char* SerializeSelf();
        /**
         * create a files object to add files to from a serialized object that was sent over the network
         */
        Shared_Files(char* seiralizedVersion);


};


#endif //HTSTORAGE_SHARED_FILES_H
