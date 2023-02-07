#include "hasher.h"
#include <openssl/sha.h>

Hasher::Hasher() {
    SHA256_Init(&sha256);
}

void
Hasher::compute(const std::string &in, bool isLastChunk) noexcept {
    if (isLastChunk) {
        SHA256_Final(output, &sha256);
        return;
    }

    SHA256_Update(&sha256, in.c_str(), in.size());
}