#include "clientConnection.h"
#include "tun_helper.h"

extern tun_helper *helper;

bool ClientConnection::status[1024] ;
std::vector<ClientConnection*> ClientConnection::connections ;
std::mutex ClientConnection::ipMutex ;

inline int getIpNum(std::string addr) 
{
    std::istringstream f(addr);
    std::string s;
    while (getline(f, s, '.'));
    return std::stoi(s);
}

void ClientConnection::setIpStatus(const std::string& addr, bool s) 
{
    ipMutex.lock() ;
    int num = getIpNum(addr) ;
    assert(status[num] != s) ;
    status[num] = s;
    ipMutex.unlock() ;
}

ClientConnection* ClientConnection::getConnectionByIp(const std::string& addr) 
{
    ipMutex.lock() ;
    return status[getIpNum(addr)]?connections[getIpNum(addr)]:NULL ;
    ipMutex.unlock() ;
}

void ClientConnection::setConnectionWithIp(const std::string& addr, ClientConnection* connection) 
{
    ipMutex.lock() ;
    int num = getIpNum(addr);
    if(!status[num]) setIpStatus(addr, true) ;
    printf("setting connection map with ip %s\n", addr.c_str()) ;
    connections[num] = connection;
    ipMutex.unlock() ;
}

std::string ClientConnection::getFreeIP() {
    ipMutex.lock() ;
    for (int i = 2; i < 1024; i++) 
        if (!status[i])
            return std::string("10.128.0.") + std::to_string(i);
    ipMutex.unlock() ;
    printf("Error: no avaliable ip left.\n") ;
    exit(1) ;
}

int ClientConnection::sendMessage(const Msg& message) 
{
    sendMutex.lock() ;
    printf("Sending an message of type %d to socket %d", message.type, socketFd) ;
    int ret = write(socketFd, &message, sizeof(Msg)) ;
    sendMutex.unlock() ;
    return ret ;
}

void ClientConnection::keepLiveThread()
{
    Msg heartBeatMsg ;
    while (heartBeatCount.load() < 3) 
    {
        sleep(20) ;
        heartBeatCount ++;
        heartBeatMsg.type = 104 ;
        heartBeatMsg.length = 0 ;
        if(sendMessage(heartBeatMsg) == -1) break ;

        printf("Sent heart beat signal to socket %d\n", socketFd) ;
    }

    close(socketFd) ;
    printf("No heartbeat signal: Socket %d has been closed.\n", socketFd) ;

    printf("Ending keeplive thread of socket %d, ipv6 address %s\n", socketFd, ipv6Address.c_str()) ;
};


void ClientConnection::writeThread() {
    while(true) {
        Msg message ; 
        wtSktQue.pop(message);
        if(sendMessage(message) == -1) break ;
        //std::cout << "Writed!" << std::endl;
    }
    printf("Ending write thread of socket %d, ipv6 address %s\n", socketFd, ipv6Address.c_str()) ;
}

void ClientConnection::push_to_write_queue(int len, char type, std::string &data) 
{
    Msg tmp(len, type, data);
    wtSktQue.push(std::move(tmp));
}

void ClientConnection::readThread()
{
    //std::cout << "Count: " << count << std::endl;
    Msg message ;
    while (heartBeatCount.load() < 3) {
        //std::cout << "PreRead" << std::endl;
        if(read(socketFd, &message, sizeof(Msg)) == -1) break ;
        printf("Readed an message from socket %d, ipv6 address %s\n", socketFd, ipv6Address.c_str()) ;
        switch(message.type)
        {
            case 100:
            {
                assert(!ipv4Address.length() || getConnectionByIp(ipv4Address) == NULL) ;
                if (!ipv4Address.length()) 
                    ipv4Address = getFreeIP();
                std::string ipInfoStr = ipv4Address+ " 0.0.0.0 202.38.120.242 8.8.8.8 202.106.0.20";

                message.type = 101 ;
                message.length = ipInfoStr.length() ;
                memcpy(message.data, ipInfoStr.c_str(), message.length) ;
                message.data[message.length] = 0 ;

                printf("Sending an message of type %d to socket %d", message.type, socketFd) ;
                sendMutex.lock() ;
                if(write(socketFd, &message, sizeof(Msg)) == -1) 
                {
                    sendMutex.unlock() ;
                    break ;
                }
                sendMutex.unlock() ;

                //socket_.write_some(buf);
                //std::cout << "PostSent " + Cliv4Addr.to_string() << std::endl;
                setIpStatus(ipv4Address, true);
                setConnectionWithIp(ipv4Address, this);
                break ;
            }
            case 104:
            {
                heartBeatCount.store(0) ;
                break ;
            }
            case 102:
            {
                helper->push_queue(std::string(message.data, message.length));
                break ;
            }
            default:
            {
                printf("error: invalid message type of connection %d.\n", socketFd) ;
                break ;
            }
        }
    }
    setIpStatus(ipv4Address, false);
    printf("Ending read thread of socket %d, ipv6 address %s\n", socketFd, ipv6Address.c_str()) ;
}

void ClientConnection::run() 
{
    printf("Creating a connection thread group on socket %d, ipv6 address %s", socketFd, ipv6Address.c_str()) ;
    std::thread(&ClientConnection::writeThread, this).detach();
    std::thread(&ClientConnection::keepLiveThread, this).detach();
    std::thread(&ClientConnection::readThread, this).detach() ;
}
