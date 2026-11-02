#include "prx/libSceLibreSsl3/libSceLibreSsl3.hpp"
#include "prx/libSceLibreSsl/include/Api.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

namespace {

void Require(bool condition, const char* message) {
    if (condition) return;
    std::fprintf(stderr, "libressl3 EVP_MD_block_size: %s\n", message);
    std::abort();
}

}

int main() {
    Require(EVP_MD_block_size_nid_postfix(EVP_md5_nid_postfix()) == 64, "MD5 has a 64-byte block");
    Require(EVP_MD_block_size_nid_postfix(EVP_sha256_nid_postfix()) == 64, "SHA-256 has a 64-byte block");
    Require(EVP_MD_block_size_nid_postfix(EVP_sha512_nid_postfix()) == 128, "SHA-512 has a 128-byte block");

    EvpMd custom;
    std::memset(&custom, 0, sizeof(custom));
    custom.blockSize = 136;
    Require(EVP_MD_block_size_nid_postfix(&custom) == 136, "the block size is read from the digest");
    custom.blockSize = -1;
    Require(EVP_MD_block_size_nid_postfix(&custom) == -1, "the block size is returned unchanged");

    bool threw = false;
    try {
        EVP_MD_block_size_nid_postfix(nullptr);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    Require(threw, "a null digest throws");

    std::puts("libressl3 EVP_MD_block_size checks passed");
    return 0;
}
