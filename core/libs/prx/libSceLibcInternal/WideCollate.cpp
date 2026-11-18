#include "prx/libSceLibcInternal/include/WideCollate.hpp"
#include "prx/libc/include/Api.hpp"
#include "prx/libc/include/GuestLocale.hpp"
#include "prx/libc/include/General.hpp"
#include "SceTypes.hpp"
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>

namespace {

const char* Characters(const char16_t* text) {
    return reinterpret_cast<const char*>(text);
}

const GuestLocale::CollateVtable& DynamicVtable(const GuestLocale::CollateFacet* self, const char* function) {
    if (self == nullptr || self->base.vtable == nullptr) throw std::invalid_argument(std::string(function) + ": null facet");
    return *reinterpret_cast<const GuestLocale::CollateVtable*>(self->base.vtable);
}

struct ScopedLocinfo {
    GuestLocale::LocinfoStorage storage{};
    bool active = false;

    explicit ScopedLocinfo(const char* localeName) {
        _ZNSt8_LocinfoC1EPKc_nid_postfix(&storage, localeName);
        active = true;
    }

    ~ScopedLocinfo() {
        if (active) _ZNSt8_LocinfoD1Ev_nid_postfix(&storage);
    }

    ScopedLocinfo(const ScopedLocinfo&) = delete;
    ScopedLocinfo& operator=(const ScopedLocinfo&) = delete;
};

void InitWideCollateFacet(GuestLocale::CollateFacet* self, std::size_t references) {
    if (self == nullptr) throw std::invalid_argument("collate<wchar_t>::collate: null object");
    self->base.vtable = &GuestWideCollateVtable_nid_no_patch()->facet;
    self->base.references = static_cast<std::uint32_t>(references);
    self->base.reserved = static_cast<std::uint32_t>(static_cast<std::uint64_t>(references) >> 32);
    self->collation = nullptr;
    self->wideCollation = nullptr;
}

void ConstructNamedWideCollate(GuestLocale::CollateFacet* self, const char* localeName, std::size_t references) {
    if (self == nullptr) throw std::invalid_argument("collate<wchar_t>::collate: null object");
    ScopedLocinfo locinfo(localeName);
    InitWideCollateFacet(self, references);
}

void ConstructBorrowedWideCollate(GuestLocale::CollateFacet* self, const GuestLocale::LocinfoStorage* locinfo, std::size_t references) {
    if (self == nullptr) throw std::invalid_argument("collate<wchar_t>::collate: null object");
    if (locinfo == nullptr) throw std::invalid_argument("collate<wchar_t>::collate: null _Locinfo");
    InitWideCollateFacet(self, references);
}

}

extern "C" {

void APS5_VABI _ZNSt7collateIwEC1Em_nid_postfix(GuestLocale::CollateFacet* self, std::size_t references) {
    ConstructNamedWideCollate(self, "C", references);
}

void APS5_VABI _ZNSt7collateIwEC2Em_nid_postfix(GuestLocale::CollateFacet* self, std::size_t references) {
    ConstructNamedWideCollate(self, "C", references);
}

void APS5_VABI _ZNSt7collateIwEC1EPKcm_nid_postfix(GuestLocale::CollateFacet* self, const char* localeName, std::size_t references) {
    ConstructNamedWideCollate(self, localeName, references);
}

void APS5_VABI _ZNSt7collateIwEC2EPKcm_nid_postfix(GuestLocale::CollateFacet* self, const char* localeName, std::size_t references) {
    ConstructNamedWideCollate(self, localeName, references);
}

void APS5_VABI _ZNSt7collateIwEC1ERKSt8_Locinfom_nid_postfix(GuestLocale::CollateFacet* self, const GuestLocale::LocinfoStorage* locinfo, std::size_t references) {
    ConstructBorrowedWideCollate(self, locinfo, references);
}

void APS5_VABI _ZNSt7collateIwEC2ERKSt8_Locinfom_nid_postfix(GuestLocale::CollateFacet* self, const GuestLocale::LocinfoStorage* locinfo, std::size_t references) {
    ConstructBorrowedWideCollate(self, locinfo, references);
}

int APS5_VABI sceLibcInternalWideCollateDoCompare_nid_no_patch(const GuestLocale::CollateFacet* self, const char16_t* first1, const char16_t* last1, const char16_t* first2, const char16_t* last2) {
    return GuestWideCollateVtable_nid_no_patch()->compare(self, Characters(first1), Characters(last1), Characters(first2), Characters(last2));
}
APS5_EXPORT("roztnFEs5Es", sceLibcInternalWideCollateDoCompare_nid_no_patch);

GuestLocale::String* APS5_VABI sceLibcInternalWideCollateDoTransform_nid_no_patch(GuestLocale::String* result, const GuestLocale::CollateFacet* self, const char16_t* first, const char16_t* last) {
    return GuestWideCollateVtable_nid_no_patch()->transform(result, self, Characters(first), Characters(last));
}
APS5_EXPORT("Zxe-nQMgxHM", sceLibcInternalWideCollateDoTransform_nid_no_patch);

std::int64_t APS5_VABI sceLibcInternalWideCollateHash_nid_no_patch(const GuestLocale::CollateFacet* self, const char16_t* first, const char16_t* last) {
    return DynamicVtable(self, "collate<wchar_t>::hash").hash(self, Characters(first), Characters(last));
}
APS5_EXPORT("entYnoIu+fc", sceLibcInternalWideCollateHash_nid_no_patch);

int APS5_VABI sceLibcInternalWideCollateCompare_nid_no_patch(const GuestLocale::CollateFacet* self, const char16_t* first1, const char16_t* last1, const char16_t* first2, const char16_t* last2) {
    return DynamicVtable(self, "collate<wchar_t>::compare").compare(self, Characters(first1), Characters(last1), Characters(first2), Characters(last2));
}
APS5_EXPORT("n-3HFZvDwBw", sceLibcInternalWideCollateCompare_nid_no_patch);

std::int64_t APS5_VABI sceLibcInternalWideCollateDoHash_nid_no_patch(const GuestLocale::CollateFacet* self, const char16_t* first, const char16_t* last) {
    return GuestWideCollateVtable_nid_no_patch()->hash(self, Characters(first), Characters(last));
}
APS5_EXPORT("cWaCDW+Dc9U", sceLibcInternalWideCollateDoHash_nid_no_patch);

GuestLocale::String* APS5_VABI sceLibcInternalWideCollateTransform_nid_no_patch(GuestLocale::String* result, const GuestLocale::CollateFacet* self, const char16_t* first, const char16_t* last) {
    return DynamicVtable(self, "collate<wchar_t>::transform").transform(result, self, Characters(first), Characters(last));
}
APS5_EXPORT("81uX7PzrtG8", sceLibcInternalWideCollateTransform_nid_no_patch);

}
