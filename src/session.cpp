#include "session.h"
#include "hasher.h"
#include <boost/bind.hpp>
#include <iostream>
#include <openssl/sha.h>

std::string
Session::readBuffer() {
    std::istream iss(&rBuffer_);
    std::string  s;
    std::getline(iss, s);
    return s;
}

void
Session::writeToBuffer(const std::string &str) {
    std::ostream output(&wBuffer_);
    output << str << "\n";
    do_write();
}

void
Session::do_read() {
    auto self(shared_from_this());
    auto callback = [this, self](boost::system::error_code ec, int size) {
        if (!ec) {
            handle(true /*is the last chunk*/);
            return;
        }

        if (ec == boost::asio::error::not_found) {
            handle(false /*is not a last chunk */);
            return;
        }

        std::cerr << "Error while reading data from client:\n\t" << ec.message()
                  << "\nfor session with session id " << sessionId_
                  << ". Closing this session." << std::endl;
    };

    boost::asio::async_read_until(
        socket_, rBuffer_, '\n',
        boost::asio::bind_executor(rwStrand_, callback));
}

void
Session::do_write() {
    auto self(shared_from_this());
    auto callback = [this, self](boost::system::error_code ec, int size) {
        if (!ec) {
            try {
                do_read();
            } catch (const std::exception &e) {
                std::cerr << "Error while calling 'Session::do_read()': "
                          << e.what();
            }
        } else {
            std::cerr << "Error while writing data to client:\n\t"
                      << ec.message() << "\nfor session with session id "
                      << sessionId_ << ". Closing this session." << std::endl;
        }
    };

    boost::asio::async_write(socket_, wBuffer_,
                             boost::asio::bind_executor(rwStrand_, callback));
}

void
Session::handle(bool lastChunk) {
    try {
    } catch (const std::exception &e) {
        std::cerr << "Error while calling handling received data: " << e.what();
    }

    std::string s = readBuffer();
    if (s.size() == 0) {
        throw std::runtime_error("No data received and no line break reched.");
        // ?????
    }

    std::cout << "read: " << s << std::endl;
    // Note: Hash computation is noexept

    hasher.compute(s, lastChunk);
    if (lastChunk) {
        writeToBuffer(hasher.getResult());
    } else {
        do_read();
    }

}

// public
Session::Session(Context &io_context, Tcp::socket socket, int sessionId)
    : ISession(sessionId)
    , rwStrand_(io_context)
    , socket_(std::move(socket))
    , rBuffer_(SHA256_DIGEST_LENGTH)
    , wBuffer_() {}

Session::~Session() {
    std::cout << "Session with session id " << sessionId_ << " closed."
              << std::endl;
}

void
Session::start() {
    std::cout << "Session with session id " << sessionId_ << " started."
              << std::endl;
    do_read();
}

std::shared_ptr<ISession>
createSession(boost::asio::io_context &    io_context,
              boost::asio::ip::tcp::socket socket, int sessionId) {
    return std::make_shared<Session>(io_context, std::move(socket), sessionId);
}
