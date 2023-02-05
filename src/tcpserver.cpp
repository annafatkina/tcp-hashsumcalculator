#include "tcpserver.h"

#include <boost/bind.hpp>
#include <functional>
#include <iostream>

void
TcpServer::do_accept() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, Tcp::socket socket) {
            if (!ec) {
                sessionFactory_(context_, std::move(socket), sessionCounter_)
                    ->start();
                sessionCounter_++;
            } else {
                std::cerr << "Failed to accept connection, error: " << ec << std::endl << std::fflush; 
            }

            do_accept();
        });
}

void
TcpServer::stop() {
    if (context_.stopped()) return;
    std::cout << "Stopping " << num_threads_ << " threads..." << std::endl;
    context_.stop();
    for (std::size_t i = 0; i < threads.size(); ++i)
        threads[i]->join();
    threads.clear();
    std::cout << "Done." << std::endl;
}

void
TcpServer::run() {
    // Create a pool of threads to run all of the io_contexts.
    threads.reserve(num_threads_);
    std::cout << "Starting " << num_threads_ << " threads..." << std::endl;
    
    for (std::size_t i = 0; i < num_threads_; ++i) {
        auto t = std::make_shared<std::thread>(
            boost::bind(&boost::asio::io_context::run, &context_));
        threads.emplace_back(std::move(t));
    }

}

TcpServer::TcpServer(short              port,
                     SessionFactoryFunc sessionFactoryFunc)
    : num_threads_(std::thread::hardware_concurrency() - 1)
    , context_()
    , signals_(context_)
    , acceptor_(context_, Tcp::endpoint(Tcp::v4(), port))
    , sessionCounter_(0)
    , sessionFactory_(sessionFactoryFunc) {

    // Register to handle the signals that indicate when the server should exit.
    // It is safe to register for the same signal multiple times in a program,
    // provided all registration for the specified signal is made through Asio.
    signals_.add(SIGINT);
    signals_.add(SIGTERM);
#if defined(SIGQUIT)
    signals_.add(SIGQUIT);
#endif   // defined(SIGQUIT)
    signals_.async_wait(boost::bind(&TcpServer::stop, this));

    do_accept();
}
