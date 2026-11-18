#include "prx/libSceLibcInternal/include/Api.hpp"
#include "prx/libc/include/Api.hpp"
#include "prx/libc/include/General.hpp"
#include "prx/libc/include/GuestLocale.hpp"
#include "SceTypes.hpp"
#include <cstddef>
#include <cstdint>
#include <stdexcept>

namespace {

const GuestLocale::WideNumpunctFacet* RequireWideNumpunct(const GuestLocale::WideNumpunctFacet* self) {
    if (self == nullptr || self->base.vtable == nullptr) throw std::invalid_argument("numpunct<wchar_t>: null facet");
    return self;
}

const GuestLocale::WideNumpunctVtable& WidePunctuation(const GuestLocale::WideNumpunctFacet* self) {
    return *reinterpret_cast<const GuestLocale::WideNumpunctVtable*>(self->base.vtable);
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

void ConstructWide(GuestLocale::WideNumpunctFacet* self, std::size_t references) {
    if (self == nullptr) throw std::invalid_argument("numpunct<wchar_t>::numpunct: null object");
    self->base.vtable = GuestWideNumpunctVtable_nid_no_patch();
    self->base.references = static_cast<std::uint32_t>(references);
    self->base.reserved = static_cast<std::uint32_t>(static_cast<std::uint64_t>(references) >> 32);
}

void ConstructDefaultWide(GuestLocale::WideNumpunctFacet* self, std::size_t references) {
    if (self == nullptr) throw std::invalid_argument("numpunct<wchar_t>::numpunct: null object");
    ScopedLocinfo locinfo("C");
    ConstructWide(self, references);
    LibcInitWideNumpunct_nid_no_patch(self, false);
    if (self->thousandsSep == 0) self->thousandsSep = u',';
}

void ConstructWideFromLocinfo(GuestLocale::WideNumpunctFacet* self, const GuestLocale::LocinfoStorage* locinfo, std::size_t references, bool isDefault) {
    if (self == nullptr) throw std::invalid_argument("numpunct<wchar_t>::numpunct: null object");
    if (locinfo == nullptr) throw std::invalid_argument("numpunct<wchar_t>::numpunct: null _Locinfo");
    ConstructWide(self, references);
    LibcInitWideNumpunct_nid_no_patch(self, isDefault);
}

void ConstructNamedWide(GuestLocale::WideNumpunctFacet* self, const char* localeName, std::size_t references, bool isDefault) {
    if (self == nullptr) throw std::invalid_argument("numpunct<wchar_t>::numpunct: null object");
    ScopedLocinfo locinfo(localeName);
    ConstructWide(self, references);
    LibcInitWideNumpunct_nid_no_patch(self, isDefault);
}

void RequireWideObject(const GuestLocale::WideNumpunctFacet* self) {
    if (self == nullptr) throw std::invalid_argument("numpunct<wchar_t>::~numpunct: null object");
}

}

extern "C" {

void APS5_VABI _ZNSt8numpunctIwEC1Em_nid_postfix(GuestLocale::WideNumpunctFacet* self, std::size_t references) {
    ConstructDefaultWide(self, references);
}

void APS5_VABI _ZNSt8numpunctIwEC2Em_nid_postfix(GuestLocale::WideNumpunctFacet* self, std::size_t references) {
    ConstructDefaultWide(self, references);
}

void APS5_VABI _ZNSt8numpunctIwEC1ERKSt8_Locinfomb_nid_postfix(GuestLocale::WideNumpunctFacet* self, const GuestLocale::LocinfoStorage* locinfo, std::size_t references, bool isDefault) {
    ConstructWideFromLocinfo(self, locinfo, references, isDefault);
}

void APS5_VABI _ZNSt8numpunctIwEC2ERKSt8_Locinfomb_nid_postfix(GuestLocale::WideNumpunctFacet* self, const GuestLocale::LocinfoStorage* locinfo, std::size_t references, bool isDefault) {
    ConstructWideFromLocinfo(self, locinfo, references, isDefault);
}

void APS5_VABI _ZNSt8numpunctIwEC1EPKcmb_nid_postfix(GuestLocale::WideNumpunctFacet* self, const char* localeName, std::size_t references, bool isDefault) {
    ConstructNamedWide(self, localeName, references, isDefault);
}

void APS5_VABI _ZNSt8numpunctIwEC2EPKcmb_nid_postfix(GuestLocale::WideNumpunctFacet* self, const char* localeName, std::size_t references, bool isDefault) {
    ConstructNamedWide(self, localeName, references, isDefault);
}

void APS5_VABI _ZNSt8numpunctIwE5_InitERKSt8_Locinfob_nid_postfix(GuestLocale::WideNumpunctFacet* self, const GuestLocale::LocinfoStorage* locinfo, bool isDefault) {
    if (self == nullptr) throw std::invalid_argument("numpunct<wchar_t>::_Init: null object");
    if (locinfo == nullptr) throw std::invalid_argument("numpunct<wchar_t>::_Init: null _Locinfo");
    LibcInitWideNumpunct_nid_no_patch(self, isDefault);
}

void APS5_VABI _ZNSt8numpunctIwED1Ev_nid_postfix(GuestLocale::WideNumpunctFacet* self) {
    RequireWideObject(self);
}

void APS5_VABI _ZNSt8numpunctIwED2Ev_nid_postfix(GuestLocale::WideNumpunctFacet* self) {
    RequireWideObject(self);
}

void APS5_VABI _ZNSt8numpunctIwED0Ev_nid_postfix(GuestLocale::WideNumpunctFacet* self) {
    RequireWideObject(self);
    _ZdlPv_nid_postfix(self);
}


char APS5_VABI sceLibcInternalNumpunctDoDecimalPoint_nid_no_patch(const GuestLocale::NumpunctFacet* self) {
    return NumpunctDoDecimalPoint_nid_no_patch(self);
}
APS5_EXPORT("JXzQGOtumdM", sceLibcInternalNumpunctDoDecimalPoint_nid_no_patch);

char APS5_VABI sceLibcInternalNumpunctDoThousandsSep_nid_no_patch(const GuestLocale::NumpunctFacet* self) {
    return NumpunctDoThousandsSep_nid_no_patch(self);
}
APS5_EXPORT("zv1EMhI7R1c", sceLibcInternalNumpunctDoThousandsSep_nid_no_patch);

GuestLocale::String* APS5_VABI sceLibcInternalNumpunctDoGrouping_nid_no_patch(GuestLocale::String* result, const GuestLocale::NumpunctFacet* self) {
    return NumpunctDoGrouping_nid_no_patch(result, self);
}
APS5_EXPORT("cnNz2ftNwEU", sceLibcInternalNumpunctDoGrouping_nid_no_patch);

GuestLocale::String* APS5_VABI sceLibcInternalNumpunctDoFalsename_nid_no_patch(GuestLocale::String* result, const GuestLocale::NumpunctFacet* self) {
    return NumpunctDoFalsename_nid_no_patch(result, self);
}
APS5_EXPORT("ozLi0i4r6ds", sceLibcInternalNumpunctDoFalsename_nid_no_patch);

GuestLocale::String* APS5_VABI sceLibcInternalNumpunctDoTruename_nid_no_patch(GuestLocale::String* result, const GuestLocale::NumpunctFacet* self) {
    return NumpunctDoTruename_nid_no_patch(result, self);
}
APS5_EXPORT("nRf0VQ++OEw", sceLibcInternalNumpunctDoTruename_nid_no_patch);

char APS5_VABI sceLibcInternalNumpunctDecimalPoint_nid_no_patch(const GuestLocale::NumpunctFacet* self) {
    return NumpunctDecimalPoint_nid_no_patch(self);
}
APS5_EXPORT("klWxQ2nKAHY", sceLibcInternalNumpunctDecimalPoint_nid_no_patch);

char APS5_VABI sceLibcInternalNumpunctThousandsSep_nid_no_patch(const GuestLocale::NumpunctFacet* self) {
    return NumpunctThousandsSep_nid_no_patch(self);
}
APS5_EXPORT("QGSIlqfIU2c", sceLibcInternalNumpunctThousandsSep_nid_no_patch);

GuestLocale::String* APS5_VABI sceLibcInternalNumpunctGrouping_nid_no_patch(GuestLocale::String* result, const GuestLocale::NumpunctFacet* self) {
    return NumpunctGrouping_nid_no_patch(result, self);
}
APS5_EXPORT("JWplGh2O0Rs", sceLibcInternalNumpunctGrouping_nid_no_patch);

GuestLocale::String* APS5_VABI sceLibcInternalNumpunctFalsename_nid_no_patch(GuestLocale::String* result, const GuestLocale::NumpunctFacet* self) {
    return NumpunctFalsename_nid_no_patch(result, self);
}
APS5_EXPORT("3+VwUA8-QPI", sceLibcInternalNumpunctFalsename_nid_no_patch);

GuestLocale::String* APS5_VABI sceLibcInternalNumpunctTruename_nid_no_patch(GuestLocale::String* result, const GuestLocale::NumpunctFacet* self) {
    return NumpunctTruename_nid_no_patch(result, self);
}
APS5_EXPORT("fXUuZEw7C24", sceLibcInternalNumpunctTruename_nid_no_patch);

std::uint16_t APS5_VABI sceLibcInternalWideNumpunctDoDecimalPoint_nid_no_patch(const GuestLocale::WideNumpunctFacet* self) {
    return RequireWideNumpunct(self)->decimalPoint;
}
APS5_EXPORT("mzRlAVX65hQ", sceLibcInternalWideNumpunctDoDecimalPoint_nid_no_patch);

std::uint16_t APS5_VABI sceLibcInternalWideNumpunctDoThousandsSep_nid_no_patch(const GuestLocale::WideNumpunctFacet* self) {
    return RequireWideNumpunct(self)->thousandsSep;
}
APS5_EXPORT("Utj8Sh5L0jE", sceLibcInternalWideNumpunctDoThousandsSep_nid_no_patch);

GuestLocale::String* APS5_VABI sceLibcInternalWideNumpunctDoGrouping_nid_no_patch(GuestLocale::String* result, const GuestLocale::WideNumpunctFacet* self) {
    return LibcAssignCString_nid_no_patch(result, RequireWideNumpunct(self)->grouping);
}
APS5_EXPORT("2BmJdX269kI", sceLibcInternalWideNumpunctDoGrouping_nid_no_patch);

GuestLocale::String* APS5_VABI sceLibcInternalWideNumpunctDoFalsename_nid_no_patch(GuestLocale::String* result, const GuestLocale::WideNumpunctFacet* self) {
    return LibcAssignWideCString_nid_no_patch(result, RequireWideNumpunct(self)->falsename);
}
APS5_EXPORT("-amctzWbEtw", sceLibcInternalWideNumpunctDoFalsename_nid_no_patch);

GuestLocale::String* APS5_VABI sceLibcInternalWideNumpunctDoTruename_nid_no_patch(GuestLocale::String* result, const GuestLocale::WideNumpunctFacet* self) {
    return LibcAssignWideCString_nid_no_patch(result, RequireWideNumpunct(self)->truename);
}
APS5_EXPORT("nvSsAW7tcX8", sceLibcInternalWideNumpunctDoTruename_nid_no_patch);

std::uint16_t APS5_VABI sceLibcInternalWideNumpunctDecimalPoint_nid_no_patch(const GuestLocale::WideNumpunctFacet* self) {
    const auto* facet = RequireWideNumpunct(self);
    return WidePunctuation(facet).decimalPoint(facet);
}
APS5_EXPORT("leSFwTZZuE4", sceLibcInternalWideNumpunctDecimalPoint_nid_no_patch);

std::uint16_t APS5_VABI sceLibcInternalWideNumpunctThousandsSep_nid_no_patch(const GuestLocale::WideNumpunctFacet* self) {
    const auto* facet = RequireWideNumpunct(self);
    return WidePunctuation(facet).thousandsSep(facet);
}
APS5_EXPORT("2Olt9gqOauQ", sceLibcInternalWideNumpunctThousandsSep_nid_no_patch);

GuestLocale::String* APS5_VABI sceLibcInternalWideNumpunctGrouping_nid_no_patch(GuestLocale::String* result, const GuestLocale::WideNumpunctFacet* self) {
    const auto* facet = RequireWideNumpunct(self);
    return WidePunctuation(facet).grouping(result, facet);
}
APS5_EXPORT("VsJCpXqMPJU", sceLibcInternalWideNumpunctGrouping_nid_no_patch);

GuestLocale::String* APS5_VABI sceLibcInternalWideNumpunctFalsename_nid_no_patch(GuestLocale::String* result, const GuestLocale::WideNumpunctFacet* self) {
    const auto* facet = RequireWideNumpunct(self);
    return WidePunctuation(facet).falsename(result, facet);
}
APS5_EXPORT("LDbKkgI-TZg", sceLibcInternalWideNumpunctFalsename_nid_no_patch);

GuestLocale::String* APS5_VABI sceLibcInternalWideNumpunctTruename_nid_no_patch(GuestLocale::String* result, const GuestLocale::WideNumpunctFacet* self) {
    const auto* facet = RequireWideNumpunct(self);
    return WidePunctuation(facet).truename(result, facet);
}
APS5_EXPORT("3M20pLo9Gdk", sceLibcInternalWideNumpunctTruename_nid_no_patch);

}
