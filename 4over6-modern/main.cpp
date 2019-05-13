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
#include "tun_helper.h"
#include "tcp_server.h"
//using boost::asio::ip::tcp;
namespace as = boost::asio;

class tcp_connection;
class tun_helper;



tun_helper *helper;

int main()
{
    helper = new tun_helper(std::string("tun5"));
    helper->start();
    //test_tun();
    try {
        boost::asio::io_service io_service;
        tcp_server server(io_service);
        io_service.run();
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}