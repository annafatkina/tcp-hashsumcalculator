#include "hasher.h"
#include <cstring>
#include <openssl/sha.h>

Hasher::Hasher()
    : inited(false) {}

void
Hasher::compute(const std::string &in, bool isLastChunk) {
    if (!inited) {
        SHA256_Init(&sha256);
        inited = true;
    }

    SHA256_Update(&sha256, in.c_str(), strlen(in.c_str()));

    if (isLastChunk) {
        SHA256_Final(output, &sha256);
        inited = false;
        return;
    }
}

std::string
Hasher::getResult() {
    std::stringstream shastr;
    shastr << std::hex << std::setfill('0');
    for (const auto &byte : output) {
        shastr << std::setw(2) << (int) byte;
    }

    return shastr.str();
}

int
Hasher::getChunkSize() const {
    return SHA256_DIGEST_LENGTH;
}

std::shared_ptr<IHasher>
createHasher() {
    return std::make_shared<Hasher>();
}
