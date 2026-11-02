#include "prx/libSceLibreSsl3/libSceLibreSsl3.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

namespace {

void Require(bool condition, const char* message) {
    if (condition) return;
    std::fprintf(stderr, "libressl3 BIO accessors: %s\n", message);
    std::abort();
}

template<typename Call>
bool ThrowsInvalidArgument(Call call) {
    try {
        call();
    } catch (const std::invalid_argument&) {
        return true;
    }
    return false;
}

LibreSsl3Bio Filled() {
    LibreSsl3Bio bio;
    std::memset(&bio, 0xA5, sizeof(bio));
    return bio;
}

bool SameExcept(const LibreSsl3Bio& left, const LibreSsl3Bio& right, std::size_t offset, std::size_t size) {
    const auto* a = reinterpret_cast<const unsigned char*>(&left);
    const auto* b = reinterpret_cast<const unsigned char*>(&right);
    for (std::size_t i = 0; i < sizeof(LibreSsl3Bio); ++i)
        if ((i < offset || i >= offset + size) && a[i] != b[i]) return false;
    return true;
}

}

int main() {
    int payload = 7;
    LibreSsl3Bio bio = Filled();
    bio.data = &payload;
    Require(BIO_get_data_nid_postfix(&bio) == &payload, "BIO_get_data returns ptr");
    bio.data = nullptr;
    Require(BIO_get_data_nid_postfix(&bio) == nullptr, "BIO_get_data returns a null ptr");

    LibreSsl3Bio before = Filled();
    bio = before;
    BIO_set_init_nid_postfix(&bio, 1);
    Require(bio.initialized == 1, "BIO_set_init stores the value");
    BIO_set_init_nid_postfix(&bio, -3);
    Require(bio.initialized == -3, "BIO_set_init stores any int");
    Require(SameExcept(bio, before, offsetof(LibreSsl3Bio, initialized), sizeof(int)), "BIO_set_init writes only init");

    bio = before;
    BIO_set_shutdown_nid_postfix(&bio, 0);
    Require(bio.shutdown == 0 && BIO_get_shutdown_nid_postfix(&bio) == 0, "BIO_set_shutdown stores 0");
    BIO_set_shutdown_nid_postfix(&bio, 1);
    Require(bio.shutdown == 1 && BIO_get_shutdown_nid_postfix(&bio) == 1, "BIO_set_shutdown stores 1");
    Require(SameExcept(bio, before, offsetof(LibreSsl3Bio, shutdown), sizeof(int)), "BIO_set_shutdown writes only shutdown");

    bio = before;
    bio.flags = 0x0F;
    BIO_set_flags_nid_postfix(&bio, 0x30);
    Require(bio.flags == 0x3F, "BIO_set_flags ors the flags in");
    BIO_set_flags_nid_postfix(&bio, 0);
    Require(bio.flags == 0x3F, "BIO_set_flags with 0 keeps the flags");
    BIO_clear_flags_nid_postfix(&bio, 0x05);
    Require(bio.flags == 0x3A, "BIO_clear_flags clears only the given bits");
    BIO_clear_flags_nid_postfix(&bio, -1);
    Require(bio.flags == 0, "BIO_clear_flags with all bits clears everything");
    LibreSsl3Bio flagsBefore = before;
    flagsBefore.flags = 0;
    Require(SameExcept(bio, flagsBefore, offsetof(LibreSsl3Bio, flags), sizeof(int)), "the flag calls write only flags");

    Require(ThrowsInvalidArgument([] { BIO_get_data_nid_postfix(nullptr); }), "BIO_get_data rejects a null BIO");
    Require(ThrowsInvalidArgument([] { BIO_set_init_nid_postfix(nullptr, 1); }), "BIO_set_init rejects a null BIO");
    Require(ThrowsInvalidArgument([] { BIO_get_shutdown_nid_postfix(nullptr); }), "BIO_get_shutdown rejects a null BIO");
    Require(ThrowsInvalidArgument([] { BIO_set_shutdown_nid_postfix(nullptr, 1); }), "BIO_set_shutdown rejects a null BIO");
    Require(ThrowsInvalidArgument([] { BIO_set_flags_nid_postfix(nullptr, 1); }), "BIO_set_flags rejects a null BIO");
    Require(ThrowsInvalidArgument([] { BIO_clear_flags_nid_postfix(nullptr, 1); }), "BIO_clear_flags rejects a null BIO");

    std::puts("libressl3 BIO accessor checks passed");
    return 0;
}
