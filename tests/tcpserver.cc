#include <boost/asio.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <ihasher.h>
#include <isession.h>
#include <tcpserver.h>

class TestClient {
    // This class implements a simple test client which runs on a localhost.

    int                          port_;
    boost::asio::io_context      io_context_;
    boost::asio::ip::tcp::socket sock_;
    boost::asio::streambuf       buffer_;

  public:
    // Create 'TestClient' object with the specified 'port'.
    TestClient(int port)
        : port_(port)
        , io_context_()
        , sock_(io_context_)
        , buffer_() {}

    // Open a connection to localhost server.
    void run() {
        try {
            boost::asio::ip::tcp::resolver resolver(io_context_);
            boost::asio::connect(
                sock_, resolver.resolve("localhost", std::to_string(port_)));
        } catch (std::exception &e) {
            std::cerr << "Exception: " << e.what() << "\n";
        }
    }

    // Send the specified 'string' message to server.
    void send(const std::string &string) {
        size_t request_length = string.size();
        boost::asio::write(sock_,
                           boost::asio::buffer(string.c_str(), request_length));
    }

    // Destroy this object.
    ~TestClient() { io_context_.stop(); }
};

class MockSession;

std::mutex                                sessionsMtx;
std::vector<std::shared_ptr<MockSession>> sessions;

int
getSessionCounter() {
    std::lock_guard<std::mutex> lg(sessionsMtx);
    return sessions.size();
}

class MockSession : public ISession {
    // This is a mock session class for 'TcpServer' testing.

    std::string mockString;

  public:
    MOCK_METHOD(std::string, readBuffer, (), (override));
    MOCK_METHOD(void, writeToBuffer, (const std::string &str), (override));
    MOCK_METHOD(void, do_read, (), (override));
    MOCK_METHOD(void, do_write, (), (override));
    MOCK_METHOD(void, handle, (bool lastChunk), (override));
    MOCK_METHOD(void, start, (), (override));

    MockSession(boost::asio::io_context &    io_context,
                boost::asio::ip::tcp::socket socket, int sessionId)
        : ISession(sessionId) {}

    ~MockSession() {}

    void setMockString(const std::string &str) { mockString = str; }
};

// Create 'MockSession' session.
std::shared_ptr<ISession>
createMockSession(boost::asio::io_context &    io_context,
                  boost::asio::ip::tcp::socket socket, int sessionId) {
    std::lock_guard<std::mutex> lg(sessionsMtx);
    auto                        session =
        std::make_shared<MockSession>(io_context, std::move(socket), sessionId);
    sessions.emplace_back(session);
    return session;
}

// Throw an error while creating a session.
std::shared_ptr<ISession>
createErrorMockSession(boost::asio::io_context &    io_context,
                       boost::asio::ip::tcp::socket socket, int sessionId) {
    throw std::runtime_error("Error creating session");
}

// Create, run and stop the server
TEST(TcpServerTests, CreateServer) {
    int port = 1234;
    testing::internal::CaptureStdout();

    // Run tcp server with mock sessions
    auto server = std::make_unique<TcpServer>(port, &createMockSession);
    server->run();
    server->stop();

    std::string threadsNum =
        std::to_string(std::thread::hardware_concurrency());

    auto stdout = testing::internal::GetCapturedStdout();
    EXPECT_EQ(stdout,
              "Starting " + threadsNum + " threads...\nTcp Server stopped.\n");
}

// Run the server, then add a single connection. Expect the corresponding mock
// session method calls.
TEST(TcpServerTests, RunServerSingleConnection) {
    int         port       = 1234;
    std::string mockString = "dummy";

    testing::internal::CaptureStderr();
    {
        // Run tcp server with mock sessions
        auto server = std::make_unique<TcpServer>(port, &createMockSession);
        server->run();

        // No active session
        int noActiveSession = getSessionCounter();
        EXPECT_EQ(noActiveSession, 0);

        // Connect test client
        TestClient client(port);
        client.run();

        // Sleep to avoid a race
        sleep(1);

        // One active session now
        int oneActiveSession = getSessionCounter();
        EXPECT_EQ(oneActiveSession, 1);

        auto firstSession = sessions[0];
        firstSession->setMockString(mockString);

        // send dummy string
        client.send(mockString);

        ON_CALL(*firstSession, readBuffer())
            .WillByDefault(::testing::Return(mockString));

        // Shared ptrs issue
        testing::Mock::AllowLeak(firstSession.get());
    }

    sessions.clear();

    // Check that there was no runtime error
    EXPECT_EQ(testing::internal::GetCapturedStderr(), "");
}

// Run the server with a session creation function throwing an error
TEST(TcpServerTests, UnhappyPathCreateConnection) {
    int         port       = 1234;
    std::string mockString = "dummy";

    testing::internal::CaptureStderr();

    // Run tcp server with mock sessions
    auto server = std::make_unique<TcpServer>(port, &createErrorMockSession);
    server->run();

    // No active session
    auto noActiveSession = getSessionCounter();
    EXPECT_EQ(noActiveSession, 0);

    // Connect test client
    TestClient client(port);
    client.run();

    // Sleep to avoid a race
    sleep(1);

    // Check the runtime error
    auto stderr = testing::internal::GetCapturedStderr();
    EXPECT_EQ(
        stderr,
        "Failed to create session with a given session factory method, error: "
        "Error creating session");

    sessions.clear();
}
