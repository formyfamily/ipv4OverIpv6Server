//
// Created by root on 17-5-15.
//

#include "tools.h"
#include <memory>
as::ip::address mkAddr(const std::string &str) {

    return as::ip::address::from_string((std::to_string(int(str[0]) < 0 ? int(str[0]) + 256 : int(str[0])) + "."
                                         + std::to_string(int(str[1]) < 0 ? int(str[1]) + 256 : int(str[1])) + "."
                                         + std::to_string(int(str[2]) < 0 ? int(str[2]) + 256 : int(str[2])) + "."
                                         + std::to_string(int(str[3]) < 0 ? int(str[3]) + 256 : int(str[3]))));
}

std::string mkAddrStr(const std::string &str) {
    return (std::to_string(int(str[0]) < 0 ? int(str[0]) + 256 : int(str[0])) + "."
                                         + std::to_string(int(str[1]) < 0 ? int(str[1]) + 256 : int(str[1])) + "."
                                         + std::to_string(int(str[2]) < 0 ? int(str[2]) + 256 : int(str[2])) + "."
                                         + std::to_string(int(str[3]) < 0 ? int(str[3]) + 256 : int(str[3])));
}
std::vector<as::const_buffer>  mkBuf_stk(int &len, char &type, std::string &data) {
    struct {int length_p; char type_p; char data_p[4096];} pkt;
    std::vector<as::const_buffer> buffers;
    pkt.length_p = len + 5;
    pkt.type_p = type;
    strcpy(pkt.data_p, data.c_str());
    auto buf = boost::asio::const_buffer(&pkt, pkt.length_p);
    //return buf;
    buffers.push_back(buf);
    return buffers;
}

std::vector<as::const_buffer> mkBuf(int &len, char &type, std::string &data) {
    std::vector<as::const_buffer> buffers;
    std::cout << "data: " + data + " len: " << len << std::endl;
    len = len + 5;
    buffers.push_back(as::const_buffer(&len, 4));
    buffers.push_back(as::const_buffer(&type, 1));
    buffers.push_back(as::const_buffer(data.c_str(), len - 5));
    return buffers;
}

std::vector<as::const_buffer> mkBuf(int &len, char &type) {
    std::vector<as::const_buffer> buffers;
    buffers.push_back(as::const_buffer(&len, 4));
    buffers.push_back(as::const_buffer(&type, 1));
    return buffers;
}