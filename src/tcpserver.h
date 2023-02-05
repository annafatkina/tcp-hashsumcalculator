#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <boost/asio.hpp>
#include "isession.h"

class TcpServer {
    using Context            = boost::asio::io_context;
    using Signals            = boost::asio::signal_set;
    using Tcp                = boost::asio::ip::tcp;
    using Acceptor           = Tcp::acceptor;
    using SessionFactoryFunc = std::function<std::shared_ptr<ISession>(
        Context &io_context, Tcp::socket socket, int sessionId)>;
    std::vector<std::shared_ptr<std::thread>> threads = {};

    unsigned int       num_threads_;
    Context            context_;
    Signals            signals_;
    Acceptor           acceptor_;
    int                sessionCounter_;
    SessionFactoryFunc sessionFactory_;

    // Accept new connection
    void do_accept();

  public:
    // Join all the threads ans stop this server.
    void stop();

    // Create 'TcpSerer' object with the specified 'io_context' and 'port'.
    TcpServer(short              port,
              SessionFactoryFunc sessionFactoryFunc = &createSession);

    // Run this server.
    void run();
    ~TcpServer() {
      stop();
    }
};

#endif   // TCPSERVER_H
