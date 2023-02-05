#ifndef HASHER_H
#define HASHER_H

#include <hash_fun.h>
#include <iomanip>
#include <iostream>
#include <string>

class Hasher {
    // This class provides a mechanism to compute a hash.

  public:
    // Return a hash for the specified 'str'.
    static std::string compute(const std::string &str) noexcept {
        size_t            res = std::hash<std::string>()(str);
        std::stringstream stream;
        stream << "0x" << std::setfill('0') << std::setw(sizeof(size_t) * 2)
               << std::hex << res;
        return stream.str();
    }
};

#endif   // HASHER_H
