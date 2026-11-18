#include "prx/libSceLibcInternal/include/Api.hpp"
#include "prx/libc/include/ApplicationHeap.hpp"
#include "prx/libc/include/Api.hpp"
#include "prx/libc/include/GuestLocale.hpp"
#include "prx/libc/include/general/VabiMacros.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>

namespace {

constexpr std::size_t WideInlineCapacity = 7;

void Require(bool condition, const char* message) {
    if (condition) return;
    std::fprintf(stderr, "libc internal wide numpunct: %s\n", message);
    std::abort();
}

std::u16string WideText(const GuestLocale::String& string) {
    const char* data = string.capacity > WideInlineCapacity ? string.pointer : string.buffer;
    std::u16string text(string.size, u'\0');
    std::memcpy(text.data(), data, string.size * sizeof(char16_t));
    Require(data[string.size * sizeof(char16_t)] == 0 && data[string.size * sizeof(char16_t) + 1] == 0, "a wide string ends with a 16-bit terminator");
    return text;
}

std::u16string CallWide(GuestLocale::String* (APS5_VABI* method)(GuestLocale::String*, const GuestLocale::WideNumpunctFacet*), const GuestLocale::WideNumpunctFacet* facet, std::size_t capacity) {
    GuestLocale::String result{};
    Require(method(&result, facet) == &result, "the wide string result is the caller's buffer");
    Require(result.capacity == capacity, "the wide string capacity counts 16-bit characters");
    std::u16string text = WideText(result);
    if (result.capacity > WideInlineCapacity) _ZdlPv_nid_postfix(result.pointer);
    return text;
}

std::string CallNarrow(GuestLocale::String* (APS5_VABI* method)(GuestLocale::String*, const GuestLocale::WideNumpunctFacet*), const GuestLocale::WideNumpunctFacet* facet) {
    GuestLocale::String result{};
    Require(method(&result, facet) == &result, "the grouping result is the caller's buffer");
    Require(result.capacity == 15, "grouping is a short narrow string");
    return {result.buffer, result.size};
}

GuestLocale::String* AssignShortWide(GuestLocale::String* result, const char16_t* text) {
    *result = {};
    while (text[result->size] != u'\0') ++result->size;
    result->capacity = WideInlineCapacity;
    std::memcpy(result->buffer, text, result->size * sizeof(char16_t));
    return result;
}

GuestLocale::String* AssignShortNarrow(GuestLocale::String* result, const char* text) {
    *result = {};
    result->size = std::strlen(text);
    result->capacity = 15;
    std::memcpy(result->buffer, text, result->size);
    return result;
}

void* APS5_VABI HeapAllocate(std::size_t bytes) { return std::malloc(bytes); }
void APS5_VABI HeapFree(void* pointer) { std::free(pointer); }
void* APS5_VABI HeapCalloc(std::size_t count, std::size_t bytes) { return std::calloc(count, bytes); }
void* APS5_VABI HeapReallocate(void* pointer, std::size_t bytes) { return std::realloc(pointer, bytes); }
void* APS5_VABI HeapAlign(std::size_t, std::size_t bytes) { return std::malloc(bytes); }
void* APS5_VABI HeapRealign(void* pointer, std::size_t bytes, std::size_t) { return std::realloc(pointer, bytes); }
int APS5_VABI HeapPosixAlign(void** pointer, std::size_t, std::size_t bytes) {
    *pointer = std::malloc(bytes);
    return *pointer == nullptr ? 12 : 0;
}

void APS5_VABI FacetNop(GuestLocale::Facet*) {}
GuestLocale::Facet* APS5_VABI FacetRelease(GuestLocale::Facet*) { return nullptr; }
std::uint16_t APS5_VABI DerivedDecimalPoint(const GuestLocale::WideNumpunctFacet*) { return 0x066b; }
std::uint16_t APS5_VABI DerivedThousandsSep(const GuestLocale::WideNumpunctFacet*) { return 0x066c; }
GuestLocale::String* APS5_VABI DerivedGrouping(GuestLocale::String* result, const GuestLocale::WideNumpunctFacet*) { return AssignShortNarrow(result, "\3\2"); }
GuestLocale::String* APS5_VABI DerivedFalsename(GuestLocale::String* result, const GuestLocale::WideNumpunctFacet*) { return AssignShortWide(result, u"nein"); }
GuestLocale::String* APS5_VABI DerivedTruename(GuestLocale::String* result, const GuestLocale::WideNumpunctFacet*) { return AssignShortWide(result, u"ja"); }
const GuestLocale::WideNumpunctVtable g_derivedVtable{{FacetNop, FacetNop, FacetNop, FacetRelease}, DerivedDecimalPoint, DerivedThousandsSep, DerivedGrouping, DerivedFalsename, DerivedTruename};

const std::uint16_t g_offName[] = {'d', 'i', 's', 'a', 'b', 'l', 'e', 'd', 0};
const std::uint16_t g_onName[] = {0x00e9, 'i', 0};

struct FacetStorage {
    alignas(GuestLocale::WideNumpunctFacet) unsigned char bytes[0x30];

    FacetStorage() { std::memset(bytes, 0xCC, sizeof(bytes)); }
    GuestLocale::WideNumpunctFacet* facet() { return reinterpret_cast<GuestLocale::WideNumpunctFacet*>(bytes); }
};

void RequireConstructed(const GuestLocale::WideNumpunctFacet* facet, const GuestLocale::FacetVtable* vtable, std::uint32_t references, std::uint16_t thousandsSep, const char* message) {
    Require(facet->base.vtable == vtable, message);
    Require(facet->base.references == references, message);
    Require(facet->base.reserved == 0, message);
    Require(sceLibcInternalWideNumpunctDoDecimalPoint_nid_no_patch(facet) == u'.', message);
    Require(sceLibcInternalWideNumpunctDoThousandsSep_nid_no_patch(facet) == thousandsSep, message);
    Require(CallNarrow(sceLibcInternalWideNumpunctDoGrouping_nid_no_patch, facet).empty(), message);
    Require(CallWide(sceLibcInternalWideNumpunctDoFalsename_nid_no_patch, facet, WideInlineCapacity) == u"false", message);
    Require(CallWide(sceLibcInternalWideNumpunctDoTruename_nid_no_patch, facet, WideInlineCapacity) == u"true", message);
}

template<typename Call> bool ThrowsInvalidArgument(Call call) {
    try {
        call();
    } catch (const std::invalid_argument&) {
        return true;
    }
    return false;
}

}

int main() {
    std::array<void*, 10> heapApi{};
    heapApi[0] = reinterpret_cast<void*>(&HeapAllocate);
    heapApi[1] = reinterpret_cast<void*>(&HeapFree);
    heapApi[2] = reinterpret_cast<void*>(&HeapCalloc);
    heapApi[3] = reinterpret_cast<void*>(&HeapReallocate);
    heapApi[4] = reinterpret_cast<void*>(&HeapAlign);
    heapApi[5] = reinterpret_cast<void*>(&HeapRealign);
    heapApi[6] = reinterpret_cast<void*>(&HeapPosixAlign);
    ApplicationHeapRegister_nid_no_patch(heapApi.data());

    GuestLocale::Facet* created = nullptr;
    Require(_ZNSt8numpunctIwE7_GetcatEPPKNSt6locale5facetEPKS1__nid_postfix(&created, nullptr) == 4 && created != nullptr, "_Getcat builds the C-locale wide facet");
    const auto* classic = reinterpret_cast<const GuestLocale::WideNumpunctFacet*>(created);

    Require(sceLibcInternalWideNumpunctDecimalPoint_nid_no_patch(classic) == u'.', "decimal_point of the C locale is L'.'");
    Require(sceLibcInternalWideNumpunctThousandsSep_nid_no_patch(classic) == u',', "thousands_sep of the C locale is L','");
    Require(CallNarrow(sceLibcInternalWideNumpunctGrouping_nid_no_patch, classic).empty(), "grouping of the C locale is empty");
    Require(CallWide(sceLibcInternalWideNumpunctFalsename_nid_no_patch, classic, WideInlineCapacity) == u"false", "falsename of the C locale is L\"false\"");
    Require(CallWide(sceLibcInternalWideNumpunctTruename_nid_no_patch, classic, WideInlineCapacity) == u"true", "truename of the C locale is L\"true\"");
    Require(sceLibcInternalWideNumpunctDoDecimalPoint_nid_no_patch(classic) == u'.', "do_decimal_point of the C locale is L'.'");
    Require(sceLibcInternalWideNumpunctDoThousandsSep_nid_no_patch(classic) == u',', "do_thousands_sep of the C locale is L','");
    Require(CallNarrow(sceLibcInternalWideNumpunctDoGrouping_nid_no_patch, classic).empty(), "do_grouping of the C locale is empty");
    Require(CallWide(sceLibcInternalWideNumpunctDoFalsename_nid_no_patch, classic, WideInlineCapacity) == u"false", "do_falsename of the C locale is L\"false\"");
    Require(CallWide(sceLibcInternalWideNumpunctDoTruename_nid_no_patch, classic, WideInlineCapacity) == u"true", "do_truename of the C locale is L\"true\"");

    const GuestLocale::WideNumpunctFacet derived{{&g_derivedVtable.facet, 1, 0}, "\4", 0x00b7, 0x2009, g_offName, g_onName};
    Require(sceLibcInternalWideNumpunctDecimalPoint_nid_no_patch(&derived) == 0x066b, "decimal_point calls the overriding do_decimal_point");
    Require(sceLibcInternalWideNumpunctThousandsSep_nid_no_patch(&derived) == 0x066c, "thousands_sep calls the overriding do_thousands_sep");
    Require(CallNarrow(sceLibcInternalWideNumpunctGrouping_nid_no_patch, &derived) == "\3\2", "grouping calls the overriding do_grouping");
    Require(CallWide(sceLibcInternalWideNumpunctFalsename_nid_no_patch, &derived, WideInlineCapacity) == u"nein", "falsename calls the overriding do_falsename");
    Require(CallWide(sceLibcInternalWideNumpunctTruename_nid_no_patch, &derived, WideInlineCapacity) == u"ja", "truename calls the overriding do_truename");
    Require(sceLibcInternalWideNumpunctDoDecimalPoint_nid_no_patch(&derived) == 0x00b7, "do_decimal_point reads the facet's 16-bit decimal point");
    Require(sceLibcInternalWideNumpunctDoThousandsSep_nid_no_patch(&derived) == 0x2009, "do_thousands_sep reads the facet's 16-bit separator");
    Require(CallNarrow(sceLibcInternalWideNumpunctDoGrouping_nid_no_patch, &derived) == "\4", "do_grouping copies the facet's narrow grouping");
    Require(CallWide(sceLibcInternalWideNumpunctDoFalsename_nid_no_patch, &derived, 15) == u"disabled", "do_falsename copies a false name longer than the inline buffer");
    Require(CallWide(sceLibcInternalWideNumpunctDoTruename_nid_no_patch, &derived, WideInlineCapacity) == u"éi", "do_truename copies the facet's 16-bit true name");

    const GuestLocale::FacetVtable* vtable = created->vtable;
    using DefaultConstructor = void (APS5_VABI*)(GuestLocale::WideNumpunctFacet*, std::size_t);
    for (DefaultConstructor construct : {_ZNSt8numpunctIwEC1Em_nid_postfix, _ZNSt8numpunctIwEC2Em_nid_postfix}) {
        FacetStorage storage;
        construct(storage.facet(), 3);
        RequireConstructed(storage.facet(), vtable, 3, u',', "numpunct(size_t) builds the C-locale wide facet with the reference count");
    }
    {
        FacetStorage storage;
        _ZNSt8numpunctIwEC1Em_nid_postfix(storage.facet(), 0x100000002ull);
        Require(storage.facet()->base.references == 2 && storage.facet()->base.reserved == 1, "numpunct(size_t) stores a 64-bit reference count");
    }

    GuestLocale::LocinfoStorage locinfo{};
    _ZNSt8_LocinfoC1EPKc_nid_postfix(&locinfo, "C");
    using LocinfoConstructor = void (APS5_VABI*)(GuestLocale::WideNumpunctFacet*, const GuestLocale::LocinfoStorage*, std::size_t, bool);
    for (LocinfoConstructor construct : {_ZNSt8numpunctIwEC1ERKSt8_Locinfomb_nid_postfix, _ZNSt8numpunctIwEC2ERKSt8_Locinfomb_nid_postfix}) {
        FacetStorage storage;
        construct(storage.facet(), &locinfo, 1, false);
        RequireConstructed(storage.facet(), vtable, 1, 0, "numpunct(const _Locinfo&) takes the C locale's empty thousands separator");
        FacetStorage defaults;
        construct(defaults.facet(), &locinfo, 0, true);
        RequireConstructed(defaults.facet(), vtable, 0, u',', "numpunct(const _Locinfo&, _Isdef) uses the default separators");
        Require(ThrowsInvalidArgument([&] { construct(storage.facet(), nullptr, 0, false); }), "numpunct(const _Locinfo&) rejects a null _Locinfo");
        Require(ThrowsInvalidArgument([&] { construct(nullptr, &locinfo, 0, false); }), "numpunct(const _Locinfo&) rejects a null object");
    }

    using NamedConstructor = void (APS5_VABI*)(GuestLocale::WideNumpunctFacet*, const char*, std::size_t, bool);
    for (NamedConstructor construct : {_ZNSt8numpunctIwEC1EPKcmb_nid_postfix, _ZNSt8numpunctIwEC2EPKcmb_nid_postfix}) {
        FacetStorage storage;
        construct(storage.facet(), "C", 5, false);
        RequireConstructed(storage.facet(), vtable, 5, 0, "numpunct(const char*) takes the C locale's empty thousands separator");
        FacetStorage defaults;
        construct(defaults.facet(), "C", 5, true);
        RequireConstructed(defaults.facet(), vtable, 5, u',', "numpunct(const char*, _Isdef) uses the default separators");
        Require(ThrowsInvalidArgument([&] { construct(storage.facet(), "fr_FR", 0, false); }), "numpunct(const char*) rejects a locale other than C");
    }

    {
        FacetStorage storage;
        auto* facet = storage.facet();
        facet->base = {&g_derivedVtable.facet, 7, 9};
        _ZNSt8numpunctIwE5_InitERKSt8_Locinfob_nid_postfix(facet, &locinfo, true);
        Require(facet->base.vtable == &g_derivedVtable.facet && facet->base.references == 7 && facet->base.reserved == 9, "_Init keeps the facet header");
        Require(facet->decimalPoint == u'.' && facet->thousandsSep == u',', "_Init with _Isdef sets the default separators");
        Require(facet->grouping != nullptr && facet->grouping[0] == 0, "_Init sets an empty grouping");
        Require(std::memcmp(facet->falsename, u"false", sizeof(u"false")) == 0 && std::memcmp(facet->truename, u"true", sizeof(u"true")) == 0, "_Init sets the false and true names");
        _ZNSt8numpunctIwE5_InitERKSt8_Locinfob_nid_postfix(facet, &locinfo, false);
        Require(facet->decimalPoint == u'.' && facet->thousandsSep == 0, "_Init without _Isdef takes the C locale's separators");
        Require(ThrowsInvalidArgument([&] { _ZNSt8numpunctIwE5_InitERKSt8_Locinfob_nid_postfix(facet, nullptr, false); }), "_Init rejects a null _Locinfo");
    }
    _ZNSt8_LocinfoD1Ev_nid_postfix(&locinfo);

    for (auto destroy : {_ZNSt8numpunctIwED1Ev_nid_postfix, _ZNSt8numpunctIwED2Ev_nid_postfix}) {
        FacetStorage storage;
        _ZNSt8numpunctIwEC1Em_nid_postfix(storage.facet(), 1);
        unsigned char before[sizeof(storage.bytes)];
        std::memcpy(before, storage.bytes, sizeof(before));
        destroy(storage.facet());
        Require(std::memcmp(before, storage.bytes, sizeof(before)) == 0, "the complete and base destructors leave the facet unchanged");
        Require(ThrowsInvalidArgument([&] { destroy(nullptr); }), "the destructors reject a null object");
    }
    auto* heapFacet = static_cast<GuestLocale::WideNumpunctFacet*>(_Znwm_nid_postfix(0x30));
    Require(heapFacet != nullptr, "operator new returns facet storage");
    _ZNSt8numpunctIwEC1Em_nid_postfix(heapFacet, 0);
    _ZNSt8numpunctIwED0Ev_nid_postfix(heapFacet);
    Require(ThrowsInvalidArgument([] { _ZNSt8numpunctIwED0Ev_nid_postfix(nullptr); }), "the deleting destructor rejects a null object");

    created->vtable->deleteObject(created);
    return 0;
}
