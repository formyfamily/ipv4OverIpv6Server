//
// Created by root on 17-5-16.
//

#include "tcp_server.h"

void tcp_server::start_accept() {
    tcp_connection::pointer new_connection =
            tcp_connection::create(acceptor_.get_io_service());

    acceptor_.async_accept(new_connection->socket(),
                           boost::bind(&tcp_server::handle_accept, this, new_connection,
                                       boost::asio::placeholders::error));
}

void tcp_server::handle_accept(tcp_connection::pointer new_connection,
                   const boost::system::error_code& error) {
    if (!error) {
        std::cout << "Start Thread" << std::endl;
        new_connection->init();
        std::cout << "New connection from " + new_connection->socket().remote_endpoint().address().to_string() << std::endl;
        auto startProc = [](tcp_connection::pointer conn) {
            conn->start();
        };
        //auto writeProc = [](tcp_connection::pointer conn) {
        //    conn->test_write();
        //};
        auto worker = std::thread(startProc, (new_connection));
        worker.detach();
        //auto worker_write = std::thread(writeProc, (new_connection));
        //worker_write.detach();
        //new_connection->start();

    }

    start_accept();
}
