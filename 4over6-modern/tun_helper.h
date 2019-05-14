#pragma once
#include "common.h"
#include "msg_queue.cpp"
#include "clientConnection.h"

class tun_helper {
public:
    //tun_helper(tun_helper const&) = delete;
    //void operator = (tun_helper const&) = delete;
    tun_helper(std::string devname);
    std::string read_some();
    void push_queue(std::string pkt);
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

