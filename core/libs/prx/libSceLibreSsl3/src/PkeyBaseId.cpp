#include "prx/libSceLibreSsl3/libSceLibreSsl3.hpp"

#include <stdexcept>

namespace {

constexpr std::int32_t NidUndef = 0;
constexpr std::int32_t NidRsaEncryption = 6;
constexpr std::int32_t NidRsa = 19;
constexpr std::int32_t NidDhKeyAgreement = 28;
constexpr std::int32_t NidDsaWithSha = 66;
constexpr std::int32_t NidDsa2 = 67;
constexpr std::int32_t NidDsaWithSha1Old = 70;
constexpr std::int32_t NidDsaWithSha1 = 113;
constexpr std::int32_t NidDsa = 116;
constexpr std::int32_t NidEcPublicKey = 408;
constexpr std::int32_t NidHmac = 855;
constexpr std::int32_t NidCmac = 894;
constexpr std::int32_t NidRsassaPss = 912;
constexpr std::int32_t NidX25519 = 950;
constexpr std::int32_t NidEd25519 = 952;

std::int32_t BaseKeyType(std::int32_t type) {
    switch (type) {
    case NidRsaEncryption:
    case NidRsa:
        return NidRsaEncryption;
    case NidDsa:
    case NidDsa2:
    case NidDsaWithSha:
    case NidDsaWithSha1:
    case NidDsaWithSha1Old:
        return NidDsa;
    case NidRsassaPss:
    case NidDhKeyAgreement:
    case NidEcPublicKey:
    case NidHmac:
    case NidCmac:
    case NidX25519:
    case NidEd25519:
        return type;
    default:
        return NidUndef;
    }
}

}

extern "C" {

int APS5_VABI EVP_PKEY_base_id_nid_postfix(const LibreSsl3EvpPkey* key) {
    if (!key)
        throw std::invalid_argument("EVP_PKEY_base_id requires a key");
    return BaseKeyType(key->type);
}

}
