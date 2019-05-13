//
// Created by root on 17-5-15.
//

#ifndef INC_4OVER6_MODERN_TCP_CONNECTION_H
#define INC_4OVER6_MODERN_TCP_CONNECTION_H
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
#include "ip_pool.h"
#include "tools.h"
#include "tun_helper.h"
#include "msg_queue.h"
//using boost::asio::ip::tcp;
namespace as = boost::asio;
class tun_helper;

class tcp_connection
        : public std::enable_shared_from_this<tcp_connection> {
public:
    typedef std::shared_ptr<tcp_connection> pointer;

    void init();

    static pointer create(as::io_service& io_service) {
        pointer tmp = pointer(new tcp_connection(io_service));
        return tmp;
    }

    as::ip::tcp::socket& socket();

    void start();

    void write_socket_thread();

    void push_to_write_queue(int len, char type, std::string &data);

private:
    tcp_connection(as::io_service& io_service)
            : socket_(io_service) {}
    void ip_reply();

    void handle_write(const boost::system::error_code& /*error*/,
                      size_t /*bytes_transferred*/) {}

    as::ip::tcp::socket socket_;
    std::string message_;
    std::atomic<int> count;
    as::ip::address Cliv4Addr;
    as::ip::address Cliv6Addr;
    MsgQueue<msg> wtSktQue;
};


#endif //INC_4OVER6_MODERN_TCP_CONNECTION_H
