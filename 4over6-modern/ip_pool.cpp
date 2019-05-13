//
// Created by root on 17-5-15.
//

#include "ip_pool.h"


void ip_pool::setAsUsed(as::ip::address addr) {
    int num = getNumFromIP(addr);
    used[num] = 1;
    return ;
}
void ip_pool::setAsFree(as::ip::address addr) {
    int num = getNumFromIP(addr);
    used[num] = 0;
    return ;
}
std::shared_ptr<tcp_connection> ip_pool::getConnByIP(as::ip::address IP) {
    int num = getNumFromIP(IP);
    //std::cout << num << std::endl;
    if (!used[num]) return NULL;
    return mapper[num];
}

std::shared_ptr<tcp_connection> ip_pool::getConnByIP(std::string IP) {
    int num = getNumFromIP(IP);
    //std::cout << num << std::endl;
    if (!used[num]) return NULL;
    return mapper[num];
}

void ip_pool::setMapper(as::ip::address IP, std::shared_ptr<tcp_connection> conn) {
    int num = getNumFromIP(IP);
    std::cout << "Set Mapper num " << num << std::endl;
    if (used[num]) {
        std::cout << "Set Map " << num << std::endl;
        mapper[num] = conn;
    }
}
as::ip::address ip_pool::getFreeIP() const {
    for (int i = 2; i < 256; i++) {
        if (!used[i]) {
            return as::ip::address::from_string(std::string("10.128.0.") + std::to_string(i));
        }
    }
    //TODO throw exception
    //throw;
}