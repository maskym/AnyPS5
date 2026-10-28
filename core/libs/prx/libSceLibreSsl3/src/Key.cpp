#include "prx/libSceLibreSsl3/libSceLibreSsl3.hpp"

#include <atomic>
#include <limits>
#include <stdexcept>

namespace {

static_assert(std::atomic_ref<std::int32_t>::required_alignment <= alignof(std::int32_t));

int Retain_nid_no_patch(std::int32_t& references) {
    std::atomic_ref<std::int32_t> count(references);
    auto previous = count.load();
    for (;;) {
        if (previous == std::numeric_limits<std::int32_t>::max())
            throw std::overflow_error("LibreSSL3 reference count overflow");
        if (count.compare_exchange_weak(previous, previous + 1))
            return previous > 0;
    }
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

int APS5_VABI BIO_up_ref_nid_postfix(LibreSsl3Bio* bio) {
    if (!bio)
        throw std::invalid_argument("BIO_up_ref requires a BIO");
    return Retain_nid_no_patch(bio->references);
}

int APS5_VABI X509_up_ref_nid_postfix(LibreSsl3X509* certificate) {
    if (!certificate)
        throw std::invalid_argument("X509_up_ref requires a certificate");
    return Retain_nid_no_patch(certificate->references);
}

int APS5_VABI X509_STORE_up_ref_nid_postfix(LibreSsl3X509Store* store) {
    if (!store)
        throw std::invalid_argument("X509_STORE_up_ref requires a store");
    return Retain_nid_no_patch(store->references);
}

int APS5_VABI RSA_up_ref_nid_postfix(LibreSsl3Rsa* rsa) {
    if (!rsa)
        throw std::invalid_argument("RSA_up_ref requires a key");
    return Retain_nid_no_patch(rsa->references);
}

int APS5_VABI SSL_up_ref_nid_postfix(LibreSsl3SslPrefix* ssl) {
    if (!ssl)
        throw std::invalid_argument("SSL_up_ref requires a connection");
    return Retain_nid_no_patch(ssl->references);
}

int APS5_VABI SSL_CTX_up_ref_nid_postfix(LibreSsl3SslContextPrefix* context) {
    if (!context)
        throw std::invalid_argument("SSL_CTX_up_ref requires a context");
    return Retain_nid_no_patch(context->references);
}

int APS5_VABI SSL_SESSION_up_ref_nid_postfix(LibreSsl3SslSessionPrefix* session) {
    if (!session)
        throw std::invalid_argument("SSL_SESSION_up_ref requires a session");
    return Retain_nid_no_patch(session->references);
}

}
