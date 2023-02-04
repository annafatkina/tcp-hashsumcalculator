#include "session.h"
#include "tcpserver.h"

#include <boost/bind.hpp>
#include <functional>
#include <iostream>

void
TcpServer::do_accept() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, Tcp::socket socket) {
            if (!ec) {
                std::make_shared<Session>(context_, std::move(socket),
                                          sessionCounter_)
                    ->start();
                sessionCounter_++;
            }

            do_accept();
        });
}

void
TcpServer::handle_stop() {
    std::cout << "Stopping " << num_threads_ << " threads..." << std::endl;
    context_.stop();
    std::cout << "Done." << std::endl;
}

void
TcpServer::run() {
    // Create a pool of threads to run all of the io_contexts.
    std::vector<std::shared_ptr<std::thread>> threads;
    threads.reserve(num_threads_);

    std::cout << "Starting " << num_threads_ << " threads..." << std::endl;
    
    for (std::size_t i = 0; i < num_threads_; ++i) {
        auto t = std::make_shared<std::thread>(
            boost::bind(&boost::asio::io_context::run, &context_));
        threads.emplace_back(std::move(t));
    }

    // Wait for all threads in the pool to exit.
    for (std::size_t i = 0; i < threads.size(); ++i)
        threads[i]->join();
}

TcpServer::TcpServer(short port)
    : num_threads_(std::thread::hardware_concurrency() - 1)
    , context_()
    , signals_(context_)
    , acceptor_(context_, Tcp::endpoint(Tcp::v4(), port))
    , sessionCounter_(0) {

    // Register to handle the signals that indicate when the server should exit.
    // It is safe to register for the same signal multiple times in a program,
    // provided all registration for the specified signal is made through Asio.
    signals_.add(SIGINT);
    signals_.add(SIGTERM);
#if defined(SIGQUIT)
    signals_.add(SIGQUIT);
#endif   // defined(SIGQUIT)
    signals_.async_wait(boost::bind(&TcpServer::handle_stop, this));

    do_accept();
}
