#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "isession.h"
#include <boost/asio.hpp>

class TcpServer {
    using Context            = boost::asio::io_context;
    using Signals            = boost::asio::signal_set;
    using Tcp                = boost::asio::ip::tcp;
    using Acceptor           = Tcp::acceptor;
    using SessionFactoryFunc = std::function<std::shared_ptr<ISession>(
        Context &io_context, Tcp::socket socket, int sessionId)>;

    std::vector<std::shared_ptr<std::thread>> threads_;
    unsigned int                              num_threads_;
    Context                                   context_;
    Signals                                   signals_;
    Acceptor                                  acceptor_;
    int                                       sessionCounter_;
    SessionFactoryFunc                        sessionFactory_;

    // Accept new connection
    void do_accept();

  public:
    // Create 'TcpServer' object with the specified 'port' and
    // 'sessionFactoryFunc'. Use 'Session' object creation function by default.
    TcpServer(short              port,
              SessionFactoryFunc sessionFactoryFunc = &createSession);

    // Destroy this object.
    ~TcpServer();

    // Join all the threads ans stop this server.
    void stop();

    // Wait until server stopped.
    void waitForStop();

    // Run this server.
    void run();
};

#endif   // TCPSERVER_H
