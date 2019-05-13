//
// Created by root on 17-5-15.
//

#ifndef INC_4OVER6_MODERN_IP_POOL_H
#define INC_4OVER6_MODERN_IP_POOL_H
#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <thread>
#include <future>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/if_tun.h>
#include <map>
#include <sstream>
#include <vector>
#include "tools.h"
//using boost::asio::ip::tcp;
namespace as = boost::asio;

class tcp_connection;
class ip_pool {
public:
    ip_pool(ip_pool const&) = delete;
    void operator = (ip_pool const&) = delete;
    static ip_pool* getInstance() {
        static ip_pool I;
        return &I;
    }
    as::ip::address getFreeIP() const;
    static int getNumFromIP(as::ip::address IP) {
        std::istringstream f(IP.to_string());
        std::string s;
        while (getline(f, s, '.'));
        return std::stoi(s);
    }
    static int getNumFromIP(std::string IP) {
        std::istringstream f(IP);
        std::string s;
        while (getline(f, s, '.'));
        return std::stoi(s);
    }
    void setAsUsed(as::ip::address addr);
    void setAsFree(as::ip::address addr);
    std::shared_ptr<tcp_connection> getConnByIP(as::ip::address IP);
    std::shared_ptr<tcp_connection> getConnByIP(std::string IP);
    void setMapper(as::ip::address IP, std::shared_ptr<tcp_connection> conn);
private:
    ip_pool(){}
    bool used[256];
    std::shared_ptr<tcp_connection> mapper[256];
};



#endif //INC_4OVER6_MODERN_IP_POOL_H
