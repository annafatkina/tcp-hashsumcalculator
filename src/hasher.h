#ifndef HASHER_H
#define HASHER_H

#include <functional>
#include <hash_fun.h>
#include <iomanip>
#include <iostream>
#include <string>

class IHasher {
  public:
    // Return a hash for the specified 'str'.
    virtual std::string compute(const std::string &str) const noexcept = 0;

    // Set the hash function by the specified 'strategy'.
    virtual void setHashComputeStrategy() = 0;

    virtual ~IHasher() = 0;
};

IHasher::~IHasher() {}

class Hasher : public IHasher {
    // This class provides a mechanism to compute a hash.

    std::function<size_t(const std::string &)> hashComputeStrategy_;

  public:
    // Create 'Hasher' object with a default hash function.
    Hasher()
        : hashComputeStrategy_(std::hash<std::string>()) {}

    // Return a hash for the specified 'str'.
    std::string compute(const std::string &str) const noexcept override {
        size_t            res = hashComputeStrategy_(str);
        std::stringstream stream;
        stream << "0x" << std::setfill('0') << std::setw(sizeof(size_t) * 2)
               << std::hex << res;
        return stream.str();
    }

    // Set the hash function by the specified 'strategy'.
    void setHashComputeStrategy(
        const std::function<size_t(const std::string &)> &strategy) override {
        hashComputeStrategy_ = strategy;
    }
};

#endif   // HASHER_H
