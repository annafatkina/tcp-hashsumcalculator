#ifndef ISESSION_H
#define ISESSION_H

class ISession : public std::enable_shared_from_this<ISession> {
    // TYPES
    using Tcp     = boost::asio::ip::tcp;
    using Context = boost::asio::io_context;

  protected:
    int sessionId_;

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
    ISession(int sessionId)
        : sessionId_(sessionId) {}

    // Destroy this object.
    virtual ~ISession() = 0;

    // Start receiving data.
    virtual void start() = 0;
};

std::shared_ptr<ISession> createSession(boost::asio::io_context     &io_context,
                                        boost::asio::ip::tcp::socket socket,
                                        int                          sessionId);

#endif // ISESSION_H