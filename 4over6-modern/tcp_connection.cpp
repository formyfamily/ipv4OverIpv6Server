//
// Created by root on 17-5-15.
//

#include "tcp_connection.h"

extern tun_helper *helper;

void tcp_connection::init() {
    Cliv6Addr = socket_.remote_endpoint().address();
    count.store(0);
}

as::ip::tcp::socket& tcp_connection::socket() {
    return socket_;
}

void tcp_connection::start() {
    auto keepliveT = [](std::atomic<int> &count, as::ip::tcp::socket &skt){
        while (count.load() < 3) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200000));
            count ++;
            int len = 0;
            char type = 104;
            auto buf = mkBuf(len, type);
            boost::system::error_code error;
            skt.write_some(buf, error);
            if (error == as::error::bad_descriptor) {
                count.store(3);
            }
            std::cout << "Sent heartbeats" << std::endl;
            //as::write(skt, buf, [&count](const boost::system::error_code& error, size_t) {
            //});
        }
        if (skt.is_open()) {
            boost::system::error_code error;
            skt.shutdown(as::ip::tcp::socket::shutdown_both, error);
        }
        std::cout << "End of keeplived" << std::endl;
        //skt.close();
    };
    auto worker = std::thread(keepliveT, std::ref(count), std::ref(socket_));
    worker.detach();

    auto writeSkt = std::thread(&tcp_connection::write_socket_thread, this);
    writeSkt.detach();

    //sleep(1);
    //std::cout << "Count: " << count << std::endl;
    boost::system::error_code error;
    char *packet_buf = new char[4096];
    while (count.load() < 3) {
        int length;
        char length_char[4];
        char type;
        //std::cout << "PreRead" << std::endl;
        int readed_length = 0;
        while (readed_length < 4) {
            readed_length += socket_.read_some(as::buffer(&length_char[readed_length], 4 - readed_length), error);
            if (error == as::error::eof || error == as::error::bad_descriptor) {
                break;
            }
        }
        if (error == as::error::eof || error == as::error::bad_descriptor) {
            break;
        }
        length = *((int*)length_char);
        //length = (length_char[0] << 24) | (length_char[1] << 16) | (length_char[2] << 8) | length_char[3];
        size_t len_type = socket_.read_some(as::buffer(&type, 1), error);
        //std::cout << "PostRead" << std::endl;
        //std::cout << "Read buffer len: " << length << " " << 4 << " " << len_type << std::endl;
        if (error == as::error::eof || error == as::error::bad_descriptor) {
            break;
        }
        if (length > 0 && len_type > 0) {
            if (type == 100) {
                ip_reply();
            } else if (type == 104) {
                //std::cout << "104" << std::endl;
                count.store(0);
            } else if (type == 102) {
                //std::cout << "102 len:" << length << std::endl;
                length -= 5;

                size_t readed_length = 0;
                while (readed_length < length) {
                    readed_length += socket_.read_some(as::buffer(&packet_buf[readed_length], length - readed_length), error);
                    if (error == as::error::eof || error == as::error::bad_descriptor) {
                        break;
                    }
                }
                helper->push_queue(std::string(packet_buf, length));
                //delete[] packet_buf;
                //auto proc = [&](char *pkt_buf, int len_pkt){
                    //helper->write_some(pkt_buf, len_pkt);
                    //helper->write_some(pkt_buf, len_pkt);
                    //(*helper).write_some(pkt_buf, len_pkt);
                //    delete[] pkt_buf;
                //};
                //auto worker = std::thread(proc, (packet_buf), len_packet);
                //worker.detach();
            } else {
                length -= 5;
                char *packet_buf = new char[length];
                size_t len_packet = socket_.read_some(as::buffer(packet_buf, length), error);
                if (error == as::error::eof || error == as::error::bad_descriptor) {
                    break;
                }
            }
        }
    }
    ip_pool::getInstance()->setAsFree(Cliv4Addr);
    std::cout << "Disconnected with " << Cliv6Addr.to_string() << std::endl;


}

void tcp_connection::ip_reply() {
    std::cout << "IP REPLY" << std::endl;
    if (Cliv4Addr.is_unspecified()) {
        Cliv4Addr = ip_pool::getInstance()->getFreeIP();
    }
    std::cout << Cliv4Addr.to_string() << std::endl;
    auto rep_str = Cliv4Addr.to_string() + std::string(" 0.0.0.0 114.114.114.114 1.2.4.8 8.8.8.8");
    std::cout << "Reply str: " << rep_str << std::endl;
    int len = rep_str.length();
    char type = 101;
    auto buf = mkBuf(len, type, rep_str);
    socket_.write_some(buf);
    //socket_.write_some(buf);
    //std::cout << "PostSent " + Cliv4Addr.to_string() << std::endl;
    ip_pool::getInstance()->setAsUsed(Cliv4Addr);
    ip_pool::getInstance()->setMapper(Cliv4Addr, shared_from_this());
/*
    as::async_write(socket_, buf, [&](const boost::system::error_code&, size_t) {
        std::cout << "PostSent " + Cliv4Addr.to_string() << std::endl;
        ip_pool::getInstance()->setAsUsed(Cliv4Addr);
        ip_pool::getInstance()->setMapper(Cliv4Addr, shared_from_this());
    });
*/
}

void tcp_connection::write_socket_thread() {
    while (1) {
        msg buf;
        wtSktQue.pop(buf);
        socket_.write_some(as::buffer(&(buf.length), 4));
        socket_.write_some(as::buffer(&(buf.type), 1));
        if (buf.type == 103) {
            socket_.write_some(as::buffer(buf.data.c_str(), buf.length - 5));
        }
        //std::cout << "Writed!" << std::endl;
    }
}

void tcp_connection::push_to_write_queue(int len, char type, std::string &data) {
    msg tmp(len, type, data);
    wtSktQue.push(std::move(tmp));
}
