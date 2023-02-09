#ifndef HASHER_H
#define HASHER_H

#include <iomanip>
#include <iostream>
#include <openssl/sha.h>
#include <string>

#include "ihasher.h"

class Hasher : public IHasher {
    // This class provides a mechanism to compute a sha256 hash.

    SHA256_CTX    sha256;
    unsigned char output[SHA256_DIGEST_LENGTH];
    unsigned char buf[8192];
    bool          inited;

  public:

    // Create 'Hasher' object.
    Hasher();

    // Compute or update a hash for the specified 'in' string. If 'isLastChunk' is true, finalize hash algo
    void compute(const std::string &in, bool isLastChunk = true);

    // Get a hex representation of the result of hash compute.
    std::string getResult() override;

    int getChunkSize() const override { return SHA256_DIGEST_LENGTH; }
};

#endif   // HASHER_H
