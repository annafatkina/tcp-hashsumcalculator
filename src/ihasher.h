#ifndef IHASHER_H
#define IHASHER_H

#include <string>

class IHasher {
    // This class provides an interface for a mechanism to compute a hash.
  public:

    // Return a hash for the specified 'str'.
    virtual void compute(const std::string &in, bool isLastChunk) noexcept = 0;

    // Get the result of hash compute as a string.
    virtual std::string getResult() = 0;

    // Destroy this object.
    virtual ~IHasher() = default;
};

#endif   // IHASHER_H