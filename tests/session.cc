#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <hasher.h>
#include <session.h>

class MockHasher : public IHasher {
public:
    MOCK_METHOD(std::string, compute, (const std::string&), (const, override));
};

TEST(SessionTests, CreateSession) {
    boost::asio::io_context context;
    boost::asio::ip::tcp::socket socket(context); 

    Session s(context, std::move(socket), 0);

    std::streambuf str;
    str << "dummy data\n";
    boost::asio::write(socket, str);

    EXPECT_EQ(0, 0); 
}
