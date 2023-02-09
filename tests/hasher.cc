#include <gtest/gtest.h>
#include <hasher.h>
#include <string>

// Check that we've got a correct conversion
TEST(HasherTests, HashFunctionCompute) {
    std::string dummy = "dummy";

    // sha256 hash for "dummy"
    std::string hash =
        "b5a2c96250612366ea272ffac6d9744aaf4b45aacd96aa7cfcb931ee3b558259";

    // Compute it via Hasher
    Hasher hasher;
    hasher.compute(dummy, true);
    std::string result = hasher.getResult();

    EXPECT_EQ(hash, result);

}
