#include <functional>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <ihasher.h>
#include <memory>
#include <session.h>

class MockHasher : public IHasher {
    // This is a mock hasher class to test 'Session' class.

  public:
    MOCK_METHOD(void, compute, (const std::string &in, bool isLastChunk),
                (override));
    MOCK_METHOD(std::string, getResult, (), (override));

    // Return a hardcoded chunk size.
    int getChunkSize() const override { return 32; }
};

class SessionWrapper : public Session {
    // This class is a thin wrapper for 'Session' object to be tested.

  public:
    // Create 'SessionWrapper' object with the specified 'context', 'socket' and
    // 'hasherFactory'. The underlying 'Session' id to be set by 0.
    SessionWrapper(boost::asio::io_context &    context,
                   boost::asio::ip::tcp::socket socket,
                   std::shared_ptr<IHasher>     hasher)
        : Session(context, std::move(socket), 0, hasher) {}

    // Call 'Session::handle'
    void handleSession() { handle(true); }
};

// Create, start and stop session successfully.
TEST(SessionTests, CreateSession) {
    {
        boost::asio::io_context      context;
        boost::asio::ip::tcp::socket socket(context);
        testing::internal::CaptureStdout();

        auto hasher = std::make_shared<MockHasher>();

        auto sessionWrapped = std::make_shared<SessionWrapper>(
            context, std::move(socket), hasher);
        sessionWrapped->start();

        EXPECT_EQ(testing::internal::GetCapturedStdout(),
                  "Session with session id 0 started.\n");
        testing::internal::CaptureStdout();
    }
    EXPECT_EQ(testing::internal::GetCapturedStdout(),
              "Session with session id 0 closed.\n");
    testing::internal::CaptureStderr();
    EXPECT_EQ(testing::internal::GetCapturedStderr(), "");
}

// Test that hash is to be computed when 'Session' handles input data
TEST(SessionTests, ComputeHash) {
    boost::asio::io_context      context;
    boost::asio::ip::tcp::socket socket(context);

    auto hasher = std::make_shared<MockHasher>();

    auto sessionWrapped =
        std::make_shared<SessionWrapper>(context, std::move(socket), hasher);

    sessionWrapped->start();

    EXPECT_CALL(*hasher, compute);
    EXPECT_CALL(*hasher, getResult);

    sessionWrapped->handleSession();
}
