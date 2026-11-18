#include "prx/libSceLibcInternal/include/Api.hpp"
#include "prx/libc/include/Api.hpp"
#include "prx/libc/include/GuestLocale.hpp"
#include "prx/libc/include/General.hpp"
#include "SceTypes.hpp"
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>

namespace {

const GuestLocale::NumPutVtable& NumPut(const GuestLocale::Facet* self) {
    return *reinterpret_cast<const GuestLocale::NumPutVtable*>(self->vtable);
}

void ConstructNumPutChar(GuestLocale::Facet* self, std::size_t refs, const char* function) {
    if (self == nullptr) throw std::invalid_argument(std::string(function) + ": null object");
    self->vtable = reinterpret_cast<const GuestLocale::FacetVtable*>(&_ZTVSt7num_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEE_nid_postfix[2]);
    self->references = static_cast<std::uint32_t>(refs);
    self->reserved = static_cast<std::uint32_t>(static_cast<std::uint64_t>(refs) >> 32);
}

void ConstructNumPutCharFromLocinfo(GuestLocale::Facet* self, const GuestLocale::LocinfoStorage* info, std::size_t refs, const char* function) {
    if (info == nullptr) throw std::invalid_argument(std::string(function) + ": null _Locinfo");
    ConstructNumPutChar(self, refs, function);
}

}

extern "C" {

GuestLocale::OutputIterator APS5_VABI sceLibcInternalNumPutPointer_nid_no_patch(const GuestLocale::Facet* self, GuestLocale::OutputIterator out, GuestLocale::IosBase* stream, char fill, const void* value) {
    return NumPut(self).putPointer(self, out, stream, fill, value);
}
APS5_EXPORT("8NVUcufbklM", sceLibcInternalNumPutPointer_nid_no_patch);

GuestLocale::OutputIterator APS5_VABI sceLibcInternalNumPutBool_nid_no_patch(const GuestLocale::Facet* self, GuestLocale::OutputIterator out, GuestLocale::IosBase* stream, char fill, bool value) {
    return NumPut(self).putBool(self, out, stream, fill, value);
}
APS5_EXPORT("nftirmo6hBg", sceLibcInternalNumPutBool_nid_no_patch);

GuestLocale::OutputIterator APS5_VABI sceLibcInternalNumPutDouble_nid_no_patch(const GuestLocale::Facet* self, GuestLocale::OutputIterator out, GuestLocale::IosBase* stream, char fill, double value) {
    return NumPut(self).putDouble(self, out, stream, fill, value);
}
APS5_EXPORT("w9NzCYAjEpQ", sceLibcInternalNumPutDouble_nid_no_patch);

GuestLocale::OutputIterator APS5_VABI sceLibcInternalNumPutLongDouble_nid_no_patch(const GuestLocale::Facet* self, GuestLocale::OutputIterator out, GuestLocale::IosBase* stream, char fill, long double value) {
    return NumPut(self).putLongDouble(self, out, stream, fill, value);
}
APS5_EXPORT("VPcTGA-LwSo", sceLibcInternalNumPutLongDouble_nid_no_patch);

GuestLocale::OutputIterator APS5_VABI sceLibcInternalNumPutLong_nid_no_patch(const GuestLocale::Facet* self, GuestLocale::OutputIterator out, GuestLocale::IosBase* stream, char fill, std::int64_t value) {
    return NumPut(self).putLong(self, out, stream, fill, value);
}
APS5_EXPORT("ffnhh0HcxJ4", sceLibcInternalNumPutLong_nid_no_patch);

GuestLocale::OutputIterator APS5_VABI sceLibcInternalNumPutUnsignedLong_nid_no_patch(const GuestLocale::Facet* self, GuestLocale::OutputIterator out, GuestLocale::IosBase* stream, char fill, std::uint64_t value) {
    return NumPut(self).putUnsignedLong(self, out, stream, fill, value);
}
APS5_EXPORT("uODuM76vS4U", sceLibcInternalNumPutUnsignedLong_nid_no_patch);

GuestLocale::OutputIterator APS5_VABI sceLibcInternalNumPutLongLong_nid_no_patch(const GuestLocale::Facet* self, GuestLocale::OutputIterator out, GuestLocale::IosBase* stream, char fill, std::int64_t value) {
    return NumPut(self).putLongLong(self, out, stream, fill, value);
}
APS5_EXPORT("NJtKruu9qOs", sceLibcInternalNumPutLongLong_nid_no_patch);

GuestLocale::OutputIterator APS5_VABI sceLibcInternalNumPutUnsignedLongLong_nid_no_patch(const GuestLocale::Facet* self, GuestLocale::OutputIterator out, GuestLocale::IosBase* stream, char fill, std::uint64_t value) {
    return NumPut(self).putUnsignedLongLong(self, out, stream, fill, value);
}
APS5_EXPORT("dep6W2Ix35s", sceLibcInternalNumPutUnsignedLongLong_nid_no_patch);

GuestLocale::OutputIterator APS5_VABI sceLibcInternalTimePutPattern_nid_no_patch(const GuestLocale::Facet* self, GuestLocale::OutputIterator out, GuestLocale::IosBase* stream, char fill, const GuestTm* time, const char* first, const char* last) {
    return LibcTimePutPattern_nid_no_patch(self, out, stream, fill, time, first, last);
}
APS5_EXPORT("j9LU8GsuEGw", sceLibcInternalTimePutPattern_nid_no_patch);

GuestLocale::OutputIterator APS5_VABI sceLibcInternalTimePut_nid_no_patch(const GuestLocale::Facet* self, GuestLocale::OutputIterator out, GuestLocale::IosBase* stream, char fill, const GuestTm* time, char format, char modifier) {
    return reinterpret_cast<const GuestLocale::TimePutVtable*>(self->vtable)->put(self, out, stream, fill, time, format, modifier);
}
APS5_EXPORT("qkuA-unH7PU", sceLibcInternalTimePut_nid_no_patch);

GuestLocale::OutputIterator APS5_VABI sceLibcInternalWideTimePutPattern_nid_no_patch(const GuestLocale::Facet* self, GuestLocale::OutputIterator out, GuestLocale::IosBase* stream, std::uint16_t fill, const GuestTm* time, const std::uint16_t* first, const std::uint16_t* last) {
    return LibcWideTimePutPattern_nid_no_patch(self, out, stream, fill, time, first, last);
}
APS5_EXPORT("Sc0lXhQG5Ko", sceLibcInternalWideTimePutPattern_nid_no_patch);

GuestLocale::OutputIterator APS5_VABI sceLibcInternalWideTimePut_nid_no_patch(const GuestLocale::Facet* self, GuestLocale::OutputIterator out, GuestLocale::IosBase* stream, std::uint16_t fill, const GuestTm* time, char format, char modifier) {
    return reinterpret_cast<const GuestLocale::WideTimePutVtable*>(self->vtable)->put(self, out, stream, fill, time, format, modifier);
}
APS5_EXPORT("Nt6eyVKm+Z4", sceLibcInternalWideTimePut_nid_no_patch);

GuestLocale::OutputIterator APS5_VABI sceLibcInternalWideTimePutDoPut_nid_no_patch(const GuestLocale::Facet* self, GuestLocale::OutputIterator out, GuestLocale::IosBase* stream, std::uint16_t fill, const GuestTm* time, char format, char modifier) {
    return LibcWideTimePutDoPut_nid_no_patch(self, out, stream, fill, time, format, modifier);
}
APS5_EXPORT("Fr7j8dMsy4s", sceLibcInternalWideTimePutDoPut_nid_no_patch);

std::size_t APS5_VABI sceLibcInternalWideTimePutGetcat_nid_no_patch(GuestLocale::Facet** facet, const GuestLocale::Implementation* const* locale) {
    return LibcWideTimePutGetcat_nid_no_patch(facet, locale);
}
APS5_EXPORT("bF2uVCqVhBY", sceLibcInternalWideTimePutGetcat_nid_no_patch);

void APS5_VABI _ZNSt7num_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEEC1Em_nid_postfix(GuestLocale::Facet* self, std::size_t refs) {
    ConstructNumPutChar(self, refs, "num_put<char>::num_put");
}

void APS5_VABI _ZNSt7num_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEEC2Em_nid_postfix(GuestLocale::Facet* self, std::size_t refs) {
    ConstructNumPutChar(self, refs, "num_put<char>::num_put");
}

void APS5_VABI _ZNSt7num_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEEC1ERKSt8_Locinfom_nid_postfix(GuestLocale::Facet* self, const GuestLocale::LocinfoStorage* info, std::size_t refs) {
    ConstructNumPutCharFromLocinfo(self, info, refs, "num_put<char>::num_put");
}

void APS5_VABI _ZNSt7num_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEEC2ERKSt8_Locinfom_nid_postfix(GuestLocale::Facet* self, const GuestLocale::LocinfoStorage* info, std::size_t refs) {
    ConstructNumPutCharFromLocinfo(self, info, refs, "num_put<char>::num_put");
}

void APS5_VABI _ZNSt7num_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEED0Ev_nid_postfix(GuestLocale::Facet* self) {
    if (self == nullptr) throw std::invalid_argument("num_put<char>::~num_put: null object");
    _ZdlPv_nid_postfix(self);
}

}
