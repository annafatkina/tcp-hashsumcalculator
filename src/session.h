#ifndef SESSION_H
#define SESSION_H

#include "boost/asio.hpp"
#include "hasher.h"
#include <memory>

class ISession : public std::enable_shared_from_this<ISession> {
    // TYPES
    using Tcp     = boost::asio::ip::tcp;
    using Context = boost::asio::io_context;
  protected:
    // Return data read from buffer.
    virtual std::string readBuffer() = 0;

    // Write the specified 'str' to buffer.
    virtual void writeToBuffer(const std::string &str) = 0;

    // Read data from the socket and schedule the callback to be called after
    // we get data.
    virtual void do_read() = 0;

    // Write data to the socket.
    virtual void do_write() = 0;

    // Process the received data.
    virtual void handle() = 0;

  public:
    // Destroy this object.
    virtual ~ISession() = 0;

    // Start receiving data.
    virtual void start() = 0;
};

class Session : public ISession {
    // TYPES
    using Tcp = boost::asio::ip::tcp;
    using Context = boost::asio::io_context;

    // DATA
    Tcp::socket            socket_;
    int                    sessionId_;
    boost::asio::streambuf rBuffer_;
    boost::asio::streambuf wBuffer_;
    Context::strand        rwStrand_;
    Hasher       hasher_;

    // PRIVATE METHODS

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
    void handle() override;

  public:
    // Create 'Session' object with the specified 'socket' and 'sessionId'.
    Session(Context &io_context, Tcp::socket socket, int sessionId);

    // Destroy this object.
    ~Session();

    // Start receiving data.
    void start() override;
};

#endif   // SESSION_H
