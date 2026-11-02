#include "prx/libSceLibreSsl3/libSceLibreSsl3.hpp"

#include <cstddef>
#include <cstdint>
#include <stdexcept>

static_assert(offsetof(LibreSsl3SslSession, sslVersion) == 0);
static_assert(offsetof(LibreSsl3SslSession, timeout) == 168);
static_assert(offsetof(LibreSsl3SslSession, time) == 176);

extern "C" {

std::int64_t APS5_VABI SSL_SESSION_get_timeout_nid_postfix(const LibreSsl3SslSession* session) {
    return session ? session->timeout : 0;
}

std::int64_t APS5_VABI SSL_SESSION_set_time_nid_postfix(LibreSsl3SslSession* session, std::int64_t time) {
    if (!session) return 0;
    session->time = time;
    return time;
}

int APS5_VABI SSL_SESSION_get_protocol_version_nid_postfix(const LibreSsl3SslSession* session) {
    if (!session) throw std::invalid_argument("SSL_SESSION_get_protocol_version requires a session");
    return session->sslVersion;
}

}
