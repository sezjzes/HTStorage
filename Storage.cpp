#include "Storage.h"
#include "pinger.h"

void Storage::allowRecieveStorage() {

    pinger p = pinger();
    strncpy(localIp, p.getLocalIp(), 15);
    localPort = 1;
}


void Storage::createResourceAd(int storage) {
    resourceAd = ResourceAd(-1, "storage", storage, 0,
                            storage, 0, localIp, localPort);
}