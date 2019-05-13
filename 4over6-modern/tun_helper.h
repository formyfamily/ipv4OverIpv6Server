//
// Created by root on 17-5-15.
//

#ifndef INC_4OVER6_MODERN_TUN_HELPER_H
#define INC_4OVER6_MODERN_TUN_HELPER_H

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
#include <boost/lockfree/spsc_queue.hpp>
#include "msg_queue.cpp"
#include "ip_pool.h"
#include "tcp_connection.h"
//using boost::asio::ip::tcp;
namespace as = boost::asio;

class tun_helper {
public:
    //tun_helper(tun_helper const&) = delete;
    //void operator = (tun_helper const&) = delete;
    tun_helper(std::string devname);
    std::string read_some();
    bool push_queue(std::string pkt);
    void tun_writer();
    int write_some(char *buf, int len);
    int write_some(const char *buf, int len);
    int write_some(std::string &buf);
    void tun_reader();
    void start();
private:
    MsgQueue<std::string> wtTUNQue;
    int fd;
};


#endif //INC_4OVER6_MODERN_TUN_HELPER_H
