#pragma once

#include <cstddef>
#include <cstdint>

struct LibreSsl3EvpPkey {
    std::int32_t type;
    std::int32_t references;
    const void* method;
    void* key;
    std::int32_t saveParameters;
};

struct LibreSsl3EcKey {
    const void* method;
    std::int32_t version;
    void* group;
    void* publicKey;
    void* privateKey;
    std::uint32_t encodingFlags;
    std::int32_t conversionForm;
    std::int32_t references;
    std::int32_t flags;
    void* extraData;
};

struct LibreSsl3Bio {
    const void* method;
    void* callback;
    void* callbackEx;
    char* callbackArgument;
    std::int32_t init;
    std::int32_t shutdown;
    std::int32_t flags;
    std::int32_t retryReason;
    std::int32_t num;
    void* pointer;
    LibreSsl3Bio* next;
    LibreSsl3Bio* previous;
    std::int32_t references;
    std::uint64_t bytesRead;
    std::uint64_t bytesWritten;
    void* extraData;
};

struct LibreSsl3X509 {
    void* certificateInfo;
    void* signatureAlgorithm;
    void* signature;
    std::int32_t valid;
    std::int32_t references;
    char* name;
    void* extraData;
    std::int64_t pathLength;
    std::uint64_t flags;
    std::uint64_t keyUsage;
    std::uint64_t extendedKeyUsage;
    std::uint64_t netscapeCertificateType;
    void* subjectKeyIdentifier;
    void* authorityKeyIdentifier;
    void* crlDistributionPoints;
    void* alternativeNames;
    void* nameConstraints;
    void* rfc3779Addresses;
    void* rfc3779AsIdentifiers;
    unsigned char hash[64];
    void* auxiliary;
};

struct LibreSsl3X509Store {
    void* objects;
    void* lookupMethods;
    void* parameters;
    void* verify;
    void* verifyCallback;
    void* checkIssued;
    void* extraData;
    std::int32_t references;
};

struct LibreSsl3Rsa {
    std::int32_t pad;
    std::int64_t version;
    const void* method;
    void* n;
    void* e;
    void* d;
    void* p;
    void* q;
    void* dmp1;
    void* dmq1;
    void* iqmp;
    void* pssParameters;
    void* extraData;
    std::int32_t references;
    std::int32_t flags;
    void* montgomeryN;
    void* montgomeryP;
    void* montgomeryQ;
    void* blinding;
    void* multiThreadBlinding;
};

struct LibreSsl3SslSessionPrefix {
    std::int32_t sslVersion;
    std::size_t masterKeyLength;
    unsigned char masterKey[48];
    std::size_t sessionIdLength;
    unsigned char sessionId[32];
    std::size_t sessionIdContextLength;
    unsigned char sessionIdContext[32];
    void* peerCertificate;
    std::int32_t peerCertificateType;
    std::int64_t verifyResult;
    std::int64_t timeout;
    std::int64_t time;
    std::int32_t references;
};

struct LibreSsl3SslMethodPrefix {
    std::int32_t dtls;
    std::int32_t server;
    std::int32_t version;
    std::uint16_t minTlsVersion;
    std::uint16_t maxTlsVersion;
};

struct LibreSsl3SslContextPrefix {
    const LibreSsl3SslMethodPrefix* method;
    const void* quicMethod;
    void* cipherList;
    void* certificateStore;
    std::int64_t sessionTimeout;
    std::int32_t references;
    void* extraCertificates;
    std::int32_t verifyMode;
    std::size_t sessionIdContextLength;
    unsigned char sessionIdContext[32];
    void* parameters;
    void* defaultPasswordCallback;
    void* defaultPasswordCallbackData;
    std::uint16_t minTlsVersion;
    std::uint16_t maxTlsVersion;
    std::uint16_t minProtoVersion;
    std::uint16_t maxProtoVersion;
};

struct LibreSsl3SslPrefix {
    std::int32_t version;
    const void* method;
    const void* quicMethod;
    void* readBio;
    void* writeBio;
    void* bufferBio;
    std::int32_t server;
    void* ssl3State;
    void* dtls1State;
    void* parameters;
    void* cipherList;
    void* certificate;
    std::size_t sessionIdContextLength;
    unsigned char sessionIdContext[32];
    void* session;
    std::int32_t verifyMode;
    std::int32_t error;
    std::int32_t errorCode;
    void* context;
    std::int64_t verifyResult;
    std::int32_t references;
};

static_assert(sizeof(LibreSsl3EvpPkey) == 32);
static_assert(offsetof(LibreSsl3EvpPkey, references) == 4);
static_assert(sizeof(LibreSsl3EcKey) == 64);
static_assert(offsetof(LibreSsl3EcKey, references) == 48);
static_assert(sizeof(LibreSsl3Bio) == 112);
static_assert(offsetof(LibreSsl3Bio, references) == 80);
static_assert(sizeof(LibreSsl3X509) == 216);
static_assert(offsetof(LibreSsl3X509, references) == 28);
static_assert(sizeof(LibreSsl3X509Store) == 64);
static_assert(offsetof(LibreSsl3X509Store, references) == 56);
static_assert(sizeof(LibreSsl3Rsa) == 152);
static_assert(offsetof(LibreSsl3Rsa, references) == 104);
static_assert(offsetof(LibreSsl3SslSessionPrefix, references) == 184);
static_assert(offsetof(LibreSsl3SslContextPrefix, references) == 40);
static_assert(offsetof(LibreSsl3SslContextPrefix, minTlsVersion) == 128);
static_assert(offsetof(LibreSsl3SslContextPrefix, maxProtoVersion) == 134);
static_assert(offsetof(LibreSsl3SslMethodPrefix, minTlsVersion) == 12);
static_assert(offsetof(LibreSsl3SslMethodPrefix, maxTlsVersion) == 14);
static_assert(offsetof(LibreSsl3SslPrefix, references) == 176);
