//
// Created by root on 17-5-15.
//

#ifndef INC_4OVER6_MODERN_TOOLS_H
#define INC_4OVER6_MODERN_TOOLS_H
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
//using boost::asio::ip::tcp;
namespace as = boost::asio;
as::ip::address mkAddr (const std::string &str);

std::string mkAddrStr(const std::string &str);
std::vector<as::const_buffer>  mkBuf_stk(int &len, char &type, std::string &data);

std::vector<as::const_buffer> mkBuf(int &len, char &type, std::string &data);

std::vector<as::const_buffer> mkBuf(int &len, char &type);

class msg {
public:
    msg() {}
    msg(int length_, char type_, std::string &data_): length(length_), type(type_) {
        data = std::move(data_);
    }
    msg(int length_, char type_, char *data_): length(length_), type(type_), data(data_, length_ - 5) {}
    msg(int length_, char type_): length(length_), type(type_){}
    int length;
    char type;
    std::string data;
};

#endif //INC_4OVER6_MODERN_TOOLS_H
