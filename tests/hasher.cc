#include <gtest/gtest.h>
#include <hasher.h>
#include <hash_fun.h>
#include <string>

// Check that we've got a correct conversion
TEST(HasherTests, HashFunctionCompute) {
    std::string dummy = "dummy";

    size_t hash = std::hash<std::string>()(dummy);

    // Compute it via Hasher
    std::string hasherString = Hasher::compute(dummy);
    std::stringstream stream(hasherString);
    size_t hashFromHasher;
    stream >> std::hex >> hashFromHasher;

    EXPECT_EQ(hash, hashFromHasher);
}
