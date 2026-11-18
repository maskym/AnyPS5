#include "prx/libSceLibcInternal/include/Api.hpp"
#include "prx/libc/include/Api.hpp"
#include "prx/libc/include/General.hpp"
#include "prx/libc/include/GuestLocale.hpp"
#include "SceTypes.hpp"
#include <cstring>
#include <stdexcept>

namespace {

const char* LocaleName(const GuestLocale::String* name) {
    if (name == nullptr) throw std::invalid_argument("_Locinfo: null locale name");
    const char* data = name->capacity > GuestLocale::StringInlineCapacity ? name->pointer : name->buffer;
    if (name->size != 0 && std::memchr(data, 0, name->size) != nullptr) throw std::invalid_argument("_Locinfo: locale name contains a null character");
    return data;
}

}

extern "C" {

void APS5_VABI sceLibcInternalLocinfoBaseConstructor_nid_no_patch(void* self, const char* localeName) {
    _ZNSt8_LocinfoC1EPKc_nid_postfix(static_cast<GuestLocale::LocinfoStorage*>(self), localeName);
}
APS5_EXPORT("pN02FS5SPgg", sceLibcInternalLocinfoBaseConstructor_nid_no_patch);

void APS5_VABI sceLibcInternalLocinfoStringConstructor_nid_no_patch(void* self, const void* name) {
    _ZNSt8_LocinfoC1EPKc_nid_postfix(static_cast<GuestLocale::LocinfoStorage*>(self), LocaleName(static_cast<const GuestLocale::String*>(name)));
}
APS5_EXPORT("2aSk2ruCP0E", sceLibcInternalLocinfoStringConstructor_nid_no_patch);

void APS5_VABI sceLibcInternalLocinfoStringBaseConstructor_nid_no_patch(void* self, const void* name) {
    _ZNSt8_LocinfoC1EPKc_nid_postfix(static_cast<GuestLocale::LocinfoStorage*>(self), LocaleName(static_cast<const GuestLocale::String*>(name)));
}
APS5_EXPORT("ReK9U6EUWuU", sceLibcInternalLocinfoStringBaseConstructor_nid_no_patch);

void APS5_VABI sceLibcInternalLocinfoBaseDestructor_nid_no_patch(void* self) {
    _ZNSt8_LocinfoD1Ev_nid_postfix(static_cast<GuestLocale::LocinfoStorage*>(self));
}
APS5_EXPORT("YXVCU6PdgZk", sceLibcInternalLocinfoBaseDestructor_nid_no_patch);

void APS5_VABI sceLibcInternalLocimpBaseCopyConstructor_nid_no_patch(void* self, const void* source) {
    _ZNSt6locale7_LocimpC1ERKS0__nid_postfix(static_cast<GuestLocale::Implementation*>(self), static_cast<const GuestLocale::Implementation*>(source));
}
APS5_EXPORT("dsJKehuajH4", sceLibcInternalLocimpBaseCopyConstructor_nid_no_patch);

}
