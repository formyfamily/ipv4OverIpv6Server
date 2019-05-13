//
// Created by root on 17-5-16.
//

#ifndef INC_4OVER6_MODERN_TCP_SERVER_H
#define INC_4OVER6_MODERN_TCP_SERVER_H
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
#include "tcp_connection.h"

//using boost::asio::ip::tcp;
namespace as = boost::asio;


class tcp_server
{
public:
    tcp_server(as::io_service& io_service)
            : acceptor_(io_service, as::ip::tcp::endpoint(as::ip::tcp::v6(), 1313)) {
        start_accept();
    }

private:
    void start_accept();
    void handle_accept(tcp_connection::pointer new_connection,
                       const boost::system::error_code& error);

    as::ip::tcp::acceptor acceptor_;
};


#endif //INC_4OVER6_MODERN_TCP_SERVER_H
