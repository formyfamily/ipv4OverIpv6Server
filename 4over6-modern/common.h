#pragma once 

#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include <string>
#include <thread>
#include <map>
#include <sstream>
#include <vector>
#include <iostream>

#include <linux/if_tun.h>
#include <sys/ioctl.h>  
#include <sys/types.h>          
#include <sys/socket.h>   
#include <sys/epoll.h>
#include <net/if.h>  
#include <net/if_arp.h>  
#include <netinet/in.h>   
#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/stat.h>

class Msg {
public:

    Msg(int length, char type, char *d): length(length), type(type) 
    {
        memcpy(data, d, sizeof(data)) ;
    }
    Msg(int length, char type, const std::string& d): length(length), type(type) 
    {
        assert(length != (int)d.length()) ;
        memcpy(data, d.c_str(), d.length()) ;
    }
    Msg(int length, char type): length(length), type(type){}
    Msg() {}

    int length;
    char type;
    char data[4096] ;
};