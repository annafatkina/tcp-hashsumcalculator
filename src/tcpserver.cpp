#include "tcpserver.h"
#include <boost/bind.hpp>
#include <functional>
#include <iostream>

void
TcpServer::do_accept() {
    acceptor_.async_accept([this](boost::system::error_code ec,
                                  Tcp::socket               socket) {
        if (!ec) {
            try {
                sessionFactory_(context_, std::move(socket), sessionCounter_)
                    ->start();
                sessionCounter_++;
            } catch (const std::exception &e) {
                std::cerr << "Failed to create session with a given session "
                             "factory method, error: "
                          << e.what();
            }
        } else {
            std::cerr << "Failed to accept connection, error: " << ec
                      << std::endl;
        }

       do_accept();
    });
}

void
TcpServer::run() {
    // Create a pool of threads to run all of the io_contexts.
    threads_.reserve(num_threads_);
    std::cout << "Starting " << num_threads_ << " threads..." << std::endl;

    for (std::size_t i = 0; i < num_threads_; ++i) {
        auto t = std::make_shared<std::thread>(
            boost::bind(&boost::asio::io_context::run, &context_));
        threads_.emplace_back(std::move(t));
    }
}

TcpServer::TcpServer(short port, SessionFactoryFunc sessionFactoryFunc)
    : context_()
    , signals_(context_)
    , acceptor_(context_, Tcp::endpoint(Tcp::v4(), port))
    , threads_()
    , sessionCounter_(0)
    , sessionFactory_(sessionFactoryFunc)
    , num_threads_(std::thread::hardware_concurrency()) {

    // Register to handle the signals that indicate when the server should exit.
    // It is safe to register for the same signal multiple times in a program,
    // provided all registration for the specified signal is made through Asio.
    signals_.add(SIGINT);
    signals_.add(SIGTERM);
#if defined(SIGQUIT)
    signals_.add(SIGQUIT);
#endif   // defined(SIGQUIT)
    signals_.async_wait(boost::bind(&Context::stop, &context_));

    do_accept();
}

TcpServer::~TcpServer() { stop(); }

void
TcpServer::stop() {
    if (acceptor_.is_open()) {
        acceptor_.close();
    }
    if (!context_.stopped()) {
        context_.stop();
        std::cout << "Tcp Server stopped." << std::endl;
    }
    waitForStop();
}

void
TcpServer::waitForStop() {
    for (std::size_t i = 0; i < threads_.size(); ++i)
        threads_[i]->join();
    threads_.clear();
}
