#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <ihasher.h>
#include <memory>
#include <session.h>

class MockHasher : public IHasher {
public:

    MockHasher() {std::cout << "Mockhasher ctor\n";}
    MOCK_METHOD(void, compute, (const std::string &in, bool isLastChunk), (override));
    std::string getResult() override {
        return "string";
    }
    int getChunkSize() const override {return 32; }
};

std::shared_ptr<MockHasher> hasher;

std::shared_ptr<IHasher> createMockHasher() {
    hasher = std::make_shared<MockHasher>();
    return hasher;
}

class SessionWrapper : public Session{
     public:

       SessionWrapper(boost::asio::io_context     &context,
                      boost::asio::ip::tcp::socket socket)
           : Session(context, std::move(socket), 0, &createMockHasher) {
        start();
    }

    void handleSession() {
        handle(true);
    }

};

// Create, start and stop session successfully.
TEST(SessionTests, CreateSession) {
std::cout << "Qowo" ;
    {
    boost::asio::io_context      context;
    boost::asio::ip::tcp::socket socket(context);
        testing::internal::CaptureStdout();
        SessionWrapper sessionWrapped(context, std::move(socket));
        EXPECT_EQ(testing::internal::GetCapturedStdout(),
              "Session with session id 0 started.\n");
        testing::internal::CaptureStdout();
    }
    EXPECT_EQ(testing::internal::GetCapturedStdout(),
              "Session with session id 0 closed.\n");
    testing::internal::CaptureStderr();
    EXPECT_EQ(testing::internal::GetCapturedStderr(), "");
}
/*
TEST(SessionTests, ComputeHash) {
    boost::asio::io_context      context;
    boost::asio::ip::tcp::socket socket(context);

    SessionWrapper sessionWrapped(context, std::move(socket));
    sessionWrapped.handleSession();
    EXPECT_CALL(*hasher, compute);
    
    //sessionWrapped.session->
    
}
*/
