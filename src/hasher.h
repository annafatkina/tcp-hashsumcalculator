#ifndef HASHER_H
#define HASHER_H

#include <iomanip>
#include <iostream>
#include <openssl/sha.h>
#include <string>

#include "ihasher.h"

class Hasher : public IHasher {
    // This class provides a mechanism to compute a hash.
    SHA256_CTX    sha256;
    unsigned char output[SHA256_DIGEST_LENGTH];
    unsigned char buf[8192];
    bool          inited;

  public:
    Hasher();

    // Return a hash for the specified 'str'.
    void compute(const std::string &in, bool isLastChunk = true) noexcept;

    // Get a hex representation of the result of hash compute.
    std::string getResult() override;
};

#endif   // HASHER_H
