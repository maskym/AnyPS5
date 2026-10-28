#include "prx/libSceLibreSsl3/libSceLibreSsl3.hpp"

#include <cstdint>
#include <stdexcept>
#include <string>

namespace {

constexpr std::uint16_t Tls11 = 0x0302;
constexpr std::uint16_t Tls12 = 0x0303;
constexpr std::uint16_t Dtls1 = 0xfeff;
constexpr std::uint16_t Dtls12 = 0xfefd;

std::uint16_t DtlsToTls_nid_no_patch(std::uint16_t version) {
    if (version == Dtls1) return Tls11;
    if (version == Dtls12) return Tls12;
    return 0;
}

std::uint16_t TlsToDtls_nid_no_patch(std::uint16_t version) {
    if (version == Tls11) return Dtls1;
    if (version == Tls12) return Dtls12;
    return 0;
}

bool ClampRange_nid_no_patch(std::uint16_t& minimum, std::uint16_t& maximum, std::uint16_t clampMinimum, std::uint16_t clampMaximum) {
    if (clampMinimum > clampMaximum || minimum > maximum) return false;
    if (clampMaximum < minimum || clampMinimum > maximum) return false;
    if (minimum < clampMinimum) minimum = clampMinimum;
    if (maximum > clampMaximum) maximum = clampMaximum;
    return true;
}

const LibreSsl3SslMethodPrefix& MethodOf_nid_no_patch(const LibreSsl3SslContextPrefix* context, const char* function) {
    if (!context)
        throw std::invalid_argument(std::string(function) + " requires a context");
    if (!context->method)
        throw std::invalid_argument(std::string(function) + " requires a context with a method");
    return *context->method;
}

int SetBound_nid_no_patch(const LibreSsl3SslMethodPrefix& method, std::uint16_t version, std::uint16_t other, bool setMinimum, std::uint16_t& tlsVersion, std::uint16_t& protoVersion) {
    if (version == 0) {
        tlsVersion = setMinimum ? method.minTlsVersion : method.maxTlsVersion;
        protoVersion = 0;
        return 1;
    }
    std::uint16_t requested = version;
    if (method.dtls && (requested = DtlsToTls_nid_no_patch(version)) == 0) return 0;
    std::uint16_t minimum = setMinimum ? requested : other;
    std::uint16_t maximum = setMinimum ? other : requested;
    if (!ClampRange_nid_no_patch(minimum, maximum, method.minTlsVersion, method.maxTlsVersion)) return 0;
    const std::uint16_t tls = setMinimum ? minimum : maximum;
    std::uint16_t proto = tls;
    if (method.dtls && (proto = TlsToDtls_nid_no_patch(tls)) == 0) return 0;
    tlsVersion = tls;
    protoVersion = proto;
    return 1;
}

} // namespace

extern "C" {

int APS5_VABI SSL_CTX_set_min_proto_version_nid_postfix(LibreSsl3SslContextPrefix* context, std::uint16_t version) {
    const auto& method = MethodOf_nid_no_patch(context, "SSL_CTX_set_min_proto_version");
    return SetBound_nid_no_patch(method, version, context->maxTlsVersion, true, context->minTlsVersion, context->minProtoVersion);
}

int APS5_VABI SSL_CTX_set_max_proto_version_nid_postfix(LibreSsl3SslContextPrefix* context, std::uint16_t version) {
    const auto& method = MethodOf_nid_no_patch(context, "SSL_CTX_set_max_proto_version");
    return SetBound_nid_no_patch(method, version, context->minTlsVersion, false, context->maxTlsVersion, context->maxProtoVersion);
}

}
