#include <gtest/gtest.h>
#include <hasher.h>
#include <session.h>
#include <memory>

class SessionWrapper {
  public:
    boost::asio::io_context      context;
    boost::asio::ip::tcp::socket socket;
    std::shared_ptr<Session>     session;

    SessionWrapper()
        : context()
        , socket(context)
        , session(std::make_shared<Session>(context, std::move(socket), 0) ){
        session->start();
    }
};

// Create, start and stop session successfully.
TEST(SessionTests, CreateSession) {

    {
        testing::internal::CaptureStdout();
        SessionWrapper s;
        EXPECT_EQ(testing::internal::GetCapturedStdout(),
              "Session with session id 0 started.\n");
        testing::internal::CaptureStdout();
    }
    EXPECT_EQ(testing::internal::GetCapturedStdout(),
              "Session with session id 0 closed.\n");
    testing::internal::CaptureStderr();
    EXPECT_EQ(testing::internal::GetCapturedStderr(), "");
}
