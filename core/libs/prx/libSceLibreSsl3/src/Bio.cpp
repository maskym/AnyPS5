#include "prx/libSceLibreSsl3/libSceLibreSsl3.hpp"

#include <cstddef>
#include <stdexcept>
#include <string>

static_assert(offsetof(LibreSsl3Bio, initialized) == 32);
static_assert(offsetof(LibreSsl3Bio, shutdown) == 36);
static_assert(offsetof(LibreSsl3Bio, flags) == 40);
static_assert(offsetof(LibreSsl3Bio, data) == 56);

namespace {

LibreSsl3Bio& RequireBio_nid_no_patch(LibreSsl3Bio* bio, const char* function) {
    if (!bio) throw std::invalid_argument(std::string(function) + " requires a BIO");
    return *bio;
}

}

extern "C" {

void* APS5_VABI BIO_get_data_nid_postfix(LibreSsl3Bio* bio) {
    return RequireBio_nid_no_patch(bio, "BIO_get_data").data;
}

void APS5_VABI BIO_set_init_nid_postfix(LibreSsl3Bio* bio, int init) {
    RequireBio_nid_no_patch(bio, "BIO_set_init").initialized = init;
}

int APS5_VABI BIO_get_shutdown_nid_postfix(LibreSsl3Bio* bio) {
    return RequireBio_nid_no_patch(bio, "BIO_get_shutdown").shutdown;
}

void APS5_VABI BIO_set_shutdown_nid_postfix(LibreSsl3Bio* bio, int shutdown) {
    RequireBio_nid_no_patch(bio, "BIO_set_shutdown").shutdown = shutdown;
}

void APS5_VABI BIO_set_flags_nid_postfix(LibreSsl3Bio* bio, int flags) {
    RequireBio_nid_no_patch(bio, "BIO_set_flags").flags |= flags;
}

void APS5_VABI BIO_clear_flags_nid_postfix(LibreSsl3Bio* bio, int flags) {
    RequireBio_nid_no_patch(bio, "BIO_clear_flags").flags &= ~flags;
}

}
