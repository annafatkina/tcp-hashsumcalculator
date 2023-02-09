#ifndef IHASHER_H
#define IHASHER_H

#include <string>
#include <memory>

class IHasher {
    // This class provides an interface for a mechanism to compute a hash.
  public:

    // Return a hash for the specified 'str'.
    virtual void compute(const std::string &in, bool isLastChunk) = 0;

    // Get the result of hash compute as a string.
    virtual std::string getResult() = 0;

    virtual int getChunkSize() const = 0;

    // Destroy this object.
    virtual ~IHasher() = default;
};

// Return a shared_ptr to a sha256 hasher.
std::shared_ptr<IHasher> createHasher();

#endif   // IHASHER_H