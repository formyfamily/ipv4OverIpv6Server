//
// Created by root on 17-5-15.
//

#include "tun_helper.h"

tun_helper::tun_helper(std::string devname) {
    ifreq ifr;
    int err;
    if ((fd = open("/dev/net/tun", O_RDWR)) == -1) {
        perror("ERROR Opening /dev/net/tun");
        exit(1);
    }
    memset(&ifr, 0, sizeof(ifr));
    devname.copy(ifr.ifr_name, IFNAMSIZ);
    ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
    if ((err = ioctl(fd, TUNSETIFF, (void *) &ifr)) == -1) {
        perror("ERROR ioctl TUNSETIFF");
        close(fd);
        exit(1);
    }
    std::cout << devname << " opened" << std::endl;
}
std::string tun_helper::read_some() {
    static char read_buf[1600];
    auto nbytes = read(fd, read_buf, sizeof(read_buf));
    return std::string(read_buf, nbytes);
}


int tun_helper::write_some(char *buf, int len) {
    std::string buffer(buf, len);
    std::cout << "Write to TUN data: " << std::string(buf, len) << std::endl;
    auto dst_ip = buffer.substr(16, 4);
    auto src_ip = buffer.substr(12, 4);
    std::cout << "src: " + mkAddr(dst_ip).to_string() << std::endl;
    std::cout << "dst: " + mkAddr(src_ip).to_string() << std::endl;
    auto nbytes = write(fd, buf, len);
    return nbytes;
}

int tun_helper::write_some(const char *buf, int len) {
    std::string buffer(buf, len);
    std::cout << "Write to TUN data: " << std::string(buf, len) << std::endl;
    auto dst_ip = buffer.substr(16, 4);
    auto src_ip = buffer.substr(12, 4);
    std::cout << "src: " + mkAddr(src_ip).to_string() << std::endl;
    std::cout << "dst: " + mkAddr(dst_ip).to_string() << std::endl;
    auto nbytes = write(fd, buf, len);
    return nbytes;
}
int tun_helper::write_some(std::string &buf) {
    return write_some(buf.c_str(), buf.length());
}

bool tun_helper::push_queue(std::string pkt) {
    wtTUNQue.push(pkt);
}
void tun_helper::tun_writer() {
    //std::cout << "Started Writer" << std::endl;
    std::string buf;
    int nbytes;
    while(1) {
        wtTUNQue.pop(buf);
        nbytes = write(fd, buf.c_str(), buf.length());
    }
}

void tun_helper::tun_reader() {
    //std::cout << "Started READER" << std::endl;
    std::string buf;
    while (true) {
        buf = read_some();
        //std::cout << "Read " << buf.size() << " bytes from TUN" << std::endl;
        std::string dst_ip = buf.substr(16, 4);
        //auto src_ip = buf.substr(12, 4);
        //std::cout << "src: " + mkAddr(src_ip).to_string() << std::endl;
        //std::cout << "dst: " + mkAddr(dst_ip).to_string() << std::endl;
        //auto conn = ip_pool::getInstance()->getConnByIP(mkAddr(dst_ip));
        auto conn = ip_pool::getInstance()->getConnByIP(mkAddrStr(dst_ip));
        if (conn == NULL)
            continue;
        int len = buf.length();
        char type = 103;
        conn->push_to_write_queue(len + 5, type, buf);

        //auto procSend = [](as::ip::address addr, std::string buf) {
        //std::cout << "Before get conn" << std::endl;
        //std::cout << "After get conn" << std::endl;
        //};
        //auto worker = std::thread(procSend, mkAddr(dst_ip), buf);
        //worker.detach();
    }
}
void tun_helper::start() {
    auto writeTUN = std::thread(&tun_helper::tun_writer, this);
    writeTUN.detach();


    auto worker = std::thread(&tun_helper::tun_reader, this);
    worker.detach();
}
