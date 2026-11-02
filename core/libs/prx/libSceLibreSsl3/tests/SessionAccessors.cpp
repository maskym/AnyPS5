#include "prx/libSceLibreSsl3/libSceLibreSsl3.hpp"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <stdexcept>

namespace {

void Require(bool condition, const char* message) {
    if (condition) return;
    std::fprintf(stderr, "libressl3 session accessors: %s\n", message);
    std::abort();
}

LibreSsl3SslSession Filled() {
    LibreSsl3SslSession session;
    std::memset(&session, 0xA5, sizeof(session));
    return session;
}

bool SameExcept(const LibreSsl3SslSession& left, const LibreSsl3SslSession& right, std::size_t offset, std::size_t size) {
    const auto* a = reinterpret_cast<const unsigned char*>(&left);
    const auto* b = reinterpret_cast<const unsigned char*>(&right);
    for (std::size_t i = 0; i < sizeof(LibreSsl3SslSession); ++i)
        if ((i < offset || i >= offset + size) && a[i] != b[i]) return false;
    return true;
}

}

int main() {
    LibreSsl3SslSession session = Filled();
    session.timeout = 7200;
    Require(SSL_SESSION_get_timeout_nid_postfix(&session) == 7200, "SSL_SESSION_get_timeout returns timeout");
    session.timeout = -1;
    Require(SSL_SESSION_get_timeout_nid_postfix(&session) == -1, "SSL_SESSION_get_timeout returns a negative timeout");
    Require(SSL_SESSION_get_timeout_nid_postfix(nullptr) == 0, "SSL_SESSION_get_timeout of a null session is 0");

    const LibreSsl3SslSession before = Filled();
    session = before;
    Require(SSL_SESSION_set_time_nid_postfix(&session, 1700000000) == 1700000000, "SSL_SESSION_set_time returns the time");
    Require(session.time == 1700000000, "SSL_SESSION_set_time stores the time");
    const std::int64_t largest = std::numeric_limits<std::int64_t>::max();
    Require(SSL_SESSION_set_time_nid_postfix(&session, largest) == largest && session.time == largest, "SSL_SESSION_set_time keeps 64 bits");
    Require(SameExcept(session, before, offsetof(LibreSsl3SslSession, time), sizeof(std::int64_t)), "SSL_SESSION_set_time writes only time");
    Require(SSL_SESSION_set_time_nid_postfix(nullptr, 5) == 0, "SSL_SESSION_set_time of a null session is 0");

    session = Filled();
    session.sslVersion = 0x0304;
    Require(SSL_SESSION_get_protocol_version_nid_postfix(&session) == 0x0304, "SSL_SESSION_get_protocol_version returns TLS 1.3");
    session.sslVersion = 0xfefd;
    Require(SSL_SESSION_get_protocol_version_nid_postfix(&session) == 0xfefd, "SSL_SESSION_get_protocol_version returns DTLS 1.2");
    bool threw = false;
    try {
        SSL_SESSION_get_protocol_version_nid_postfix(nullptr);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    Require(threw, "SSL_SESSION_get_protocol_version rejects a null session");

    std::puts("libressl3 session accessor checks passed");
    return 0;
}
