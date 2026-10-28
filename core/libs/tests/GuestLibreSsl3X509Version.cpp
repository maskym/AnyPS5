#include "prx/libSceLibreSsl3/libSceLibreSsl3.hpp"

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>

static void Require(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "%s\n", message);
        std::abort();
    }
}

struct Certificate {
    LibreSsl3X509 x509{};
    LibreSsl3X509CertificateInfo info{};
    Asn1String version{};
    std::vector<unsigned char> bytes;

    Certificate(std::int32_t type, std::vector<unsigned char> content) : bytes(std::move(content)) {
        x509.certificateInfo = &info;
        info.version = &version;
        version.type = type;
        version.length = static_cast<std::int32_t>(bytes.size());
        version.data = bytes.empty() ? nullptr : bytes.data();
    }
};

static std::int64_t Version(std::int32_t type, std::vector<unsigned char> content) {
    Certificate certificate(type, std::move(content));
    const Asn1String before = certificate.version;
    const auto value = X509_get_version_nid_postfix(&certificate.x509);
    Require(std::memcmp(&before, &certificate.version, sizeof(before)) == 0, "X509_get_version changed the integer");
    return value;
}

template<class TException>
static void RequireThrows(std::int32_t type, std::vector<unsigned char> content, const char* message) {
    bool threw = false;
    try {
        Version(type, std::move(content));
    } catch (const TException&) {
        threw = true;
    }
    Require(threw, message);
}

int main() {
    Require(Version(2, {}) == 0, "an empty integer is not zero");
    Require(Version(2, {0x00}) == 0, "version 1 is not 0");
    Require(Version(2, {0x01}) == 1, "version 2 is not 1");
    Require(Version(2, {0x02}) == 2, "version 3 is not 2");
    Require(Version(2, {0x00, 0x00, 0x02}) == 2, "leading zero bytes changed the value");
    Require(Version(2, {0x01, 0x00}) == 256, "two bytes are not read big-endian");
    Require(Version(2, {0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}) == std::numeric_limits<std::int64_t>::max(), "INT64_MAX");
    Require(Version(2, {0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}) == std::numeric_limits<std::int64_t>::max(), "INT64_MAX with a leading zero");
    Require(Version(2 | 0x100, {0x05}) == -5, "a negative integer");
    Require(Version(2 | 0x100, {0x80, 0, 0, 0, 0, 0, 0, 0}) == std::numeric_limits<std::int64_t>::min(), "INT64_MIN");
    RequireThrows<std::runtime_error>(2, {0x80, 0, 0, 0, 0, 0, 0, 0}, "a value above INT64_MAX did not throw");
    RequireThrows<std::runtime_error>(2 | 0x100, {0x80, 0, 0, 0, 0, 0, 0, 1}, "a value below INT64_MIN did not throw");
    RequireThrows<std::runtime_error>(2, {0x01, 0, 0, 0, 0, 0, 0, 0, 0}, "a nine-byte value did not throw");
    RequireThrows<std::runtime_error>(4, {0x01}, "an octet string did not throw");
    {
        Certificate certificate(2, {0x02});
        certificate.version.length = -1;
        Require(X509_get_version_nid_postfix(&certificate.x509) == -1, "a negative length is not -1");
    }
    {
        Certificate certificate(2, {});
        certificate.info.version = nullptr;
        Require(X509_get_version_nid_postfix(&certificate.x509) == 0, "an absent version is not 0");
    }
    {
        Certificate certificate(2, {});
        certificate.version.length = 1;
        bool threw = false;
        try {
            X509_get_version_nid_postfix(&certificate.x509);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        Require(threw, "content without data did not throw");
    }
    {
        Certificate certificate(2, {0x02});
        certificate.x509.certificateInfo = nullptr;
        bool threw = false;
        try {
            X509_get_version_nid_postfix(&certificate.x509);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        Require(threw, "a certificate without information did not throw");
    }
    bool threw = false;
    try {
        X509_get_version_nid_postfix(nullptr);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    Require(threw, "a null certificate did not throw");
    return 0;
}
