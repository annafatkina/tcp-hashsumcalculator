#include <boost/asio.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <isession.h>
#include <tcpserver.h>

class TestClient {
    int                          port_;
    boost::asio::io_context      io_context_;
    boost::asio::ip::tcp::socket sock_;
    boost::asio::streambuf       buffer_;

  public:
    TestClient(int port)
        : port_(port)
        , io_context_()
        , sock_(io_context_)
        , buffer_() {}

    void run() {
        try {
            boost::asio::ip::tcp::resolver resolver(io_context_);
            boost::asio::connect(
                sock_, resolver.resolve("localhost", std::to_string(port_)));
        } catch (std::exception &e) {
            std::cerr << "Exception: " << e.what() << "\n";
        }
    }

    void send(const std::string &string) {
        size_t request_length = string.size();
        boost::asio::write(sock_,
                           boost::asio::buffer(string.c_str(), request_length));
    }

    ~TestClient() { io_context_.stop(); }
};

class MockSession;

int                                       sessionCounter = 0;
std::vector<std::shared_ptr<MockSession>> sessions;

class MockSession : public ISession {
    std::string mockString;

  public:
    MOCK_METHOD(std::string, readBuffer, (), (override));
    MOCK_METHOD(void, writeToBuffer, (const std::string &str), (override));
    MOCK_METHOD(void, do_read, (), (override));
    MOCK_METHOD(void, do_write, (), (override));
    MOCK_METHOD(void, handle, (), (override));
    MOCK_METHOD(void, start, (), (override));

    MockSession(boost::asio::io_context &    io_context,
                boost::asio::ip::tcp::socket socket, int sessionId)
        : ISession(sessionId) {
        sessionCounter++;
    }

    ~MockSession() { sessionCounter--; }

    void setMockString(const std::string &str) { mockString = str; }
};

std::shared_ptr<ISession>
createMockSession(boost::asio::io_context &    io_context,
                  boost::asio::ip::tcp::socket socket, int sessionId) {
    auto session =
        std::make_shared<MockSession>(io_context, std::move(socket), sessionId);
    sessions.emplace_back(session);
    return session;
}

// Create, run and stop the server
TEST(TcpServerTests, CreateServer) {
    int port = 1234;
    testing::internal::CaptureStdout();

    // Run tcp server with mock sessions
    TcpServer server(port, &createMockSession);
    server.run();
    server.stop();

    std::string threadsNum =
        std::to_string(std::thread::hardware_concurrency() - 1);
    
    EXPECT_EQ(testing::internal::GetCapturedStdout(),
              "Starting " + threadsNum + " threads...\nTcp Server stopped.\n");
}

// Run the server, then add a single connection. Expect the corresponding mock
// session method calls.
TEST(TcpServerTests, RunServerSingleConnection) {
    int         port       = 1234;
    std::string mockString = "dummy";

    // Run tcp server with mock sessions
    TcpServer server(port, &createMockSession);
    server.run();
    // No active session
    EXPECT_EQ(sessionCounter, 0);

    // Connect test client
    TestClient client(port);
    client.run();

    // Sleep to avoid a race
    sleep(1);

    // One active session now
    EXPECT_EQ(sessionCounter, 1);

    auto firstSession = sessions[0];
    firstSession->setMockString(mockString);

    // send dummy string
    client.send(mockString);

    ON_CALL(*firstSession, readBuffer())
        .WillByDefault(::testing::Return(mockString));

    
    server.stop();
    sessions.clear();

    // Shared ptrs issue
    testing::Mock::AllowLeak(firstSession.get());
}
