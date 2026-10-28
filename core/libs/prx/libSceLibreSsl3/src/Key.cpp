#include "prx/libSceLibreSsl3/libSceLibreSsl3.hpp"

#include <atomic>
#include <limits>
#include <stdexcept>
#include <string>

namespace {

static_assert(std::atomic_ref<std::int32_t>::required_alignment <= alignof(std::int32_t));

int Retain_nid_no_patch(std::int32_t& references) {
    std::atomic_ref<std::int32_t> count(references);
    auto previous = count.load();
    for (;;) {
        if (previous == std::numeric_limits<std::int32_t>::max())
            throw std::overflow_error("LibreSSL3 key reference count overflow");
        if (count.compare_exchange_weak(previous, previous + 1))
            return previous > 0;
    }
}

constexpr std::int32_t PkeyRsa = 6;
constexpr std::int32_t PkeyDh = 28;
constexpr std::int32_t PkeyDsa = 116;
constexpr std::int32_t PkeyRsaPss = 912;

void* KeyOfType_nid_no_patch(const LibreSsl3EvpPkey* key, std::int32_t type, std::int32_t alternative, const char* function) {
    if (!key)
        throw std::invalid_argument(std::string(function) + " requires a key");
    if (key->type != type && key->type != alternative)
        throw std::runtime_error(std::string(function) + ": a key of another type adds an EVP error, and the LibreSSL3 error queue is not implemented");
    return key->key;
}

} // namespace

extern "C" {

int APS5_VABI EVP_PKEY_up_ref_nid_postfix(LibreSsl3EvpPkey* key) {
    if (!key)
        throw std::invalid_argument("EVP_PKEY_up_ref requires a key");
    return Retain_nid_no_patch(key->references);
}

int APS5_VABI EC_KEY_up_ref_nid_postfix(LibreSsl3EcKey* key) {
    if (!key)
        throw std::invalid_argument("EC_KEY_up_ref requires a key");
    return Retain_nid_no_patch(key->references);
}

void* APS5_VABI EVP_PKEY_get0_RSA_nid_postfix(LibreSsl3EvpPkey* key) {
    return KeyOfType_nid_no_patch(key, PkeyRsa, PkeyRsaPss, "EVP_PKEY_get0_RSA");
}

void* APS5_VABI EVP_PKEY_get0_DSA_nid_postfix(LibreSsl3EvpPkey* key) {
    return KeyOfType_nid_no_patch(key, PkeyDsa, PkeyDsa, "EVP_PKEY_get0_DSA");
}

void* APS5_VABI EVP_PKEY_get0_DH_nid_postfix(LibreSsl3EvpPkey* key) {
    return KeyOfType_nid_no_patch(key, PkeyDh, PkeyDh, "EVP_PKEY_get0_DH");
}

}
