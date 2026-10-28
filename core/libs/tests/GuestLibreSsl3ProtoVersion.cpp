#include "prx/libSceLibreSsl3/libSceLibreSsl3.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

static void Require(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "%s\n", message);
        std::abort();
    }
}

constexpr std::uint16_t Tls1 = 0x0301;
constexpr std::uint16_t Tls11 = 0x0302;
constexpr std::uint16_t Tls12 = 0x0303;
constexpr std::uint16_t Tls13 = 0x0304;
constexpr std::uint16_t Dtls1 = 0xfeff;
constexpr std::uint16_t Dtls12 = 0xfefd;

using Setter = int (APS5_VABI*)(LibreSsl3SslContextPrefix*, std::uint16_t);

struct Context {
    LibreSsl3SslMethodPrefix method{};
    LibreSsl3SslContextPrefix context;

    Context(bool dtls, std::uint16_t minimum, std::uint16_t maximum) {
        std::memset(&context, 0xA5, sizeof(context));
        method.dtls = dtls ? 1 : 0;
        method.minTlsVersion = minimum;
        method.maxTlsVersion = maximum;
        context.method = &method;
        context.minTlsVersion = minimum;
        context.maxTlsVersion = maximum;
        context.minProtoVersion = 0;
        context.maxProtoVersion = 0;
    }
};

static void Expect(Context& state, Setter set, std::uint16_t version, int result, std::uint16_t minTls, std::uint16_t maxTls,
                   std::uint16_t minProto, std::uint16_t maxProto, const char* message) {
    LibreSsl3SslContextPrefix before;
    std::memcpy(&before, &state.context, sizeof(before));
    Require(set(&state.context, version) == result, message);
    Require(state.context.minTlsVersion == minTls && state.context.maxTlsVersion == maxTls, message);
    Require(state.context.minProtoVersion == minProto && state.context.maxProtoVersion == maxProto, message);
    LibreSsl3SslContextPrefix rest;
    std::memcpy(&rest, &state.context, sizeof(rest));
    rest.minTlsVersion = before.minTlsVersion;
    rest.maxTlsVersion = before.maxTlsVersion;
    rest.minProtoVersion = before.minProtoVersion;
    rest.maxProtoVersion = before.maxProtoVersion;
    Require(std::memcmp(&rest, &before, sizeof(rest)) == 0, "a setter changed a field other than the four versions");
}

int main() {
    const Setter min = SSL_CTX_set_min_proto_version_nid_postfix;
    const Setter max = SSL_CTX_set_max_proto_version_nid_postfix;
    {
        Context tls(false, Tls1, Tls13);
        Expect(tls, min, Tls12, 1, Tls12, Tls13, Tls12, 0, "TLS minimum 1.2");
        Expect(tls, min, 0x0300, 1, Tls1, Tls13, Tls1, 0, "a minimum below the method is clamped");
        Expect(tls, min, 0x0305, 0, Tls1, Tls13, Tls1, 0, "a minimum above the maximum fails without changes");
        Expect(tls, max, Tls11, 1, Tls1, Tls11, Tls1, Tls11, "TLS maximum 1.1");
        Expect(tls, min, Tls12, 0, Tls1, Tls11, Tls1, Tls11, "a minimum above the current maximum fails");
        Expect(tls, max, 0x0305, 1, Tls1, Tls13, Tls1, Tls13, "a maximum above the method is clamped");
        Expect(tls, min, Tls13, 1, Tls13, Tls13, Tls13, Tls13, "a minimum equal to the maximum");
        Expect(tls, max, Tls12, 0, Tls13, Tls13, Tls13, Tls13, "a maximum below the current minimum fails");
        Expect(tls, min, 0, 1, Tls1, Tls13, 0, Tls13, "minimum 0 selects the method's minimum");
        Expect(tls, max, 0, 1, Tls1, Tls13, 0, 0, "maximum 0 selects the method's maximum");
        Expect(tls, min, Dtls12, 0, Tls1, Tls13, 0, 0, "a DTLS version is out of range for a TLS method");
    }
    {
        Context dtls(true, Tls11, Tls12);
        Expect(dtls, min, Dtls12, 1, Tls12, Tls12, Dtls12, 0, "DTLS minimum 1.2");
        Expect(dtls, min, Tls12, 0, Tls12, Tls12, Dtls12, 0, "a TLS version is rejected for a DTLS method");
        Expect(dtls, min, Dtls1, 1, Tls11, Tls12, Dtls1, 0, "DTLS minimum 1.0");
        Expect(dtls, max, Dtls1, 1, Tls11, Tls11, Dtls1, Dtls1, "DTLS maximum 1.0");
        Expect(dtls, min, Dtls12, 0, Tls11, Tls11, Dtls1, Dtls1, "a DTLS minimum above the maximum fails");
        Expect(dtls, max, 0x1234, 0, Tls11, Tls11, Dtls1, Dtls1, "an unknown DTLS version fails");
        Expect(dtls, max, 0, 1, Tls11, Tls12, Dtls1, 0, "DTLS maximum 0 selects the method's maximum");
    }
    {
        Context broken(false, Tls13, Tls1);
        broken.context.minTlsVersion = Tls1;
        broken.context.maxTlsVersion = Tls13;
        Expect(broken, min, Tls12, 0, Tls1, Tls13, 0, 0, "a method with an empty range fails");
    }
    bool threw = false;
    try {
        min(nullptr, Tls12);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    Require(threw, "a null context did not throw");
    Context noMethod(false, Tls1, Tls13);
    noMethod.context.method = nullptr;
    threw = false;
    try {
        max(&noMethod.context, Tls12);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    Require(threw, "a context without a method did not throw");
    return 0;
}
