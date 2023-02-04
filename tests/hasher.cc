#include <gtest/gtest.h>
#include <hasher.h>
#include <hash_fun.h>
#include <string>

TEST(HasherTests, DefaultFunctionCompute) {
    Hasher hasher;
    std::string dummy = "dummy";

    size_t hash = std::hash<std::string>()(dummy);

    std::string hasherString = hasher.compute(dummy);
    auto        hasherToUL   = std::stoul(hasherString.c_str(), 0, 16);

    EXPECT_EQ(hash, hasherToUL);
}

TEST(HasherTests, CustomFunc) {
    Hasher      hasher;
    size_t dummyReturn;
    hasher.setHashComputeStrategy(
        [&](const std::string &str) { return dummyReturn; });
    
    std::string dummy = "dummy";
    std::string hasherString = hasher.compute(dummy);
    auto        hasherToUL   = std::stoul(hasherString.c_str(), 0, 16);

    EXPECT_EQ(hasherToUL, dummyReturn);
}
