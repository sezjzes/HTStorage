//
// Created by Josh Szczesniak on 4/2/21.
//

#ifndef HTSTORAGE_RESOURCE_H
#define HTSTORAGE_RESOURCE_H

#include "ResourceAd.h"
class Resource {
    public:
        ResourceAd resourceAd;
        void advertiseToManager(char* managerIp, int port);
};


#endif //HTSTORAGE_RESOURCE_H
