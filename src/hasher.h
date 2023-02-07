#ifndef HASHER_H
#define HASHER_H

#include <hash_fun.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <openssl/sha.h>

class IHasher {
    // This class provides an interface for a mechanism to compute a hash.
  public:
    // Return a hash for the specified 'str'.
    virtual void compute(const std::string &in, bool isLastChunk) noexcept = 0;

    virtual ~IHasher() = default;
};

class Hasher : public IHasher {
    // This class provides a mechanism to compute a hash.
    SHA256_CTX sha256;
    unsigned char output[SHA256_DIGEST_LENGTH];
    unsigned char buf[8192];

  public:
    Hasher(); 
    
    // Return a hash for the specified 'str'.
    void compute(const std::string &in, bool isLastChunk = true) noexcept;

    std::string getResult() {
      std::stringstream shastr;
      shastr << std::hex << std::setfill('0');
      for (const auto &byte : output) {
          shastr << std::setw(2) << (int) byte;
      }
      return shastr.str();
    }
};

#endif   // HASHER_H
