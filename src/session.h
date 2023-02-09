#ifndef SESSION_H
#define SESSION_H

#include "ihasher.h"
#include "isession.h"
#include <boost/asio.hpp>
#include <memory>

class Session : public ISession {
    // This class represents a single client session.

    // TYPES
    using Tcp               = boost::asio::ip::tcp;
    using Context           = boost::asio::io_context;

    // DATA
    std::shared_ptr<IHasher> hasher_;
    Context::strand          rwStrand_;
    Tcp::socket              socket_;
    boost::asio::streambuf   rBuffer_;
    boost::asio::streambuf   wBuffer_;

  protected:
    // Return data read from buffer.
    std::string readBuffer() override;

    // Write the specified 'str' to buffer.
    void writeToBuffer(const std::string &str) override;

    // Read data from the socket and schedule the callback to be called after
    // we get data.
    void do_read() override;

    // Write data to the socket.
    void do_write() override;

    // Process the received data.
    void handle(bool lastChunk) override;

  public:
    // Create 'Session' object with the specified 'io_context', 'socket' and
    // 'sessionId'.
    Session(Context &io_context, Tcp::socket socket, int sessionId,
            std::shared_ptr<IHasher> hasher);

    // Destroy this object.
    virtual ~Session();

    // Start receiving data.
    void start() override;
};

#endif   // SESSION_H
