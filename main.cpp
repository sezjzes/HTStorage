#include <iostream>
#include "pinger.h"


int main() {
    pinger p;
    std::cout << p.ping(const_cast<char *>(&"127.0.0.1"[0])) << std::endl;
    std::cout << p.ping(const_cast<char *>(&"8.8.8.8"[0])) << std::endl;
}
