#include "common.h"
#include "clientConnection.h"
#include "tun_helper.h"
#include "server.h"

tun_helper *helper;

int main()
{
    helper = new tun_helper(std::string("tun5"));
    helper->start();

    Server server;
    server.startServer() ;

    return 0;
}