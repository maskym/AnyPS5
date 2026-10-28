#include "prx/libSceLibreSsl3/libSceLibreSsl3.hpp"

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <stdexcept>

static void Require(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "%s\n", message);
        std::abort();
    }
}

using Accessor = void* (APS5_VABI*)(LibreSsl3EvpPkey*);

static void CheckAccessor(Accessor get, std::initializer_list<std::int32_t> accepted, const char* name) {
    int marker = 0;
    for (std::int32_t type : {0, 6, 28, 116, 408, 912, 67, 70, -1}) {
        LibreSsl3EvpPkey key;
        std::memset(&key, 0xA5, sizeof(key));
        key.type = type;
        key.key = &marker;
        std::array<unsigned char, sizeof(key)> before;
        std::memcpy(before.data(), &key, sizeof(key));
        bool accepts = false;
        for (std::int32_t candidate : accepted)
            accepts = accepts || candidate == type;
        bool threw = false;
        void* result = nullptr;
        try {
            result = get(&key);
        } catch (const std::runtime_error&) {
            threw = true;
        }
        if (accepts) {
            Require(!threw && result == &marker, name);
        } else {
            Require(threw, name);
        }
        Require(std::memcmp(before.data(), &key, sizeof(key)) == 0, "an accessor changed the key");
    }
    LibreSsl3EvpPkey empty{};
    empty.type = *accepted.begin();
    Require(get(&empty) == nullptr, "a key with no object did not return its null pointer");
    bool threw = false;
    try {
        get(nullptr);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    Require(threw, "a null key did not throw");
}

int main() {
    CheckAccessor(EVP_PKEY_get0_RSA_nid_postfix, {6, 912}, "EVP_PKEY_get0_RSA type selection");
    CheckAccessor(EVP_PKEY_get0_DSA_nid_postfix, {116}, "EVP_PKEY_get0_DSA type selection");
    CheckAccessor(EVP_PKEY_get0_DH_nid_postfix, {28}, "EVP_PKEY_get0_DH type selection");
    return 0;
}
