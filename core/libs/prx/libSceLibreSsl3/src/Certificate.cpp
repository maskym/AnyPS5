#include "prx/libSceLibreSsl3/libSceLibreSsl3.hpp"

#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>

namespace {

constexpr std::int32_t Asn1Integer = 2;
constexpr std::int32_t Asn1NegativeInteger = 2 | 0x100;

static_assert(sizeof(LibreSsl3X509CertificateInfo) == 104);
static_assert(offsetof(LibreSsl3X509CertificateInfo, version) == 0);
static_assert(offsetof(LibreSsl3X509, certificateInfo) == 0);

[[noreturn]] void QueuedError_nid_no_patch(const char* reason) {
    throw std::runtime_error(std::string("ASN1_INTEGER_get: ") + reason + " adds an ASN1 error, and the LibreSSL3 error queue is not implemented");
}

std::int64_t IntegerValue_nid_no_patch(const Asn1String* integer) {
    if (!integer)
        return 0;
    if (integer->length < 0)
        return -1;
    if (integer->type != Asn1Integer && integer->type != Asn1NegativeInteger)
        QueuedError_nid_no_patch("a wrong integer type");
    if (integer->length > 0 && !integer->data)
        throw std::invalid_argument("ASN1_INTEGER_get: integer content without data");
    std::uint64_t magnitude = 0;
    for (std::int32_t index = 0; index < integer->length; ++index) {
        if (magnitude > (std::numeric_limits<std::uint64_t>::max() >> 8))
            QueuedError_nid_no_patch("a value too large");
        magnitude = magnitude << 8 | integer->data[index];
    }
    if (integer->type == Asn1NegativeInteger) {
        constexpr auto limit = static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max()) + 1;
        if (magnitude > limit)
            QueuedError_nid_no_patch("a value too small");
        return magnitude == limit ? std::numeric_limits<std::int64_t>::min() : -static_cast<std::int64_t>(magnitude);
    }
    if (magnitude > static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max()))
        QueuedError_nid_no_patch("a value too large");
    return static_cast<std::int64_t>(magnitude);
}

} // namespace

extern "C" {

std::int64_t APS5_VABI X509_get_version_nid_postfix(const LibreSsl3X509* certificate) {
    if (!certificate)
        throw std::invalid_argument("X509_get_version requires a certificate");
    if (!certificate->certificateInfo)
        throw std::invalid_argument("X509_get_version requires certificate information");
    return IntegerValue_nid_no_patch(static_cast<const Asn1String*>(certificate->certificateInfo->version));
}

}
