#include "prx/libSceLibcInternal/include/RuntimeInternals.hpp"
#include "prx/libc/include/GuestLocale.hpp"
#include <cstdio>
#include <stdexcept>
#include <type_traits>

namespace {

static_assert(std::is_same_v<decltype(&_Deletegloballocale_nid_postfix), void (APS5_VABI*)(void*)>);

unsigned failures = 0;
unsigned releaseCalls = 0;
unsigned deleteCalls = 0;
GuestLocale::Facet* releaseArgument = nullptr;
GuestLocale::Facet* deleteArgument = nullptr;
bool lastReference = false;

void Require(bool value, const char* what) {
    if (value) return;
    std::fprintf(stderr, "FAIL: %s\n", what);
    ++failures;
}

GuestLocale::Facet* APS5_VABI FakeRelease(GuestLocale::Facet* self) {
    ++releaseCalls;
    releaseArgument = self;
    return lastReference ? self : nullptr;
}

void APS5_VABI FakeDelete(GuestLocale::Facet* self) {
    ++deleteCalls;
    deleteArgument = self;
}

const GuestLocale::FacetVtable FakeVtable{nullptr, FakeDelete, nullptr, FakeRelease};
const GuestLocale::FacetVtable NoReleaseVtable{nullptr, FakeDelete, nullptr, nullptr};
const GuestLocale::FacetVtable NoDeleteVtable{nullptr, nullptr, nullptr, FakeRelease};

void Reset(bool last) {
    releaseCalls = 0;
    deleteCalls = 0;
    releaseArgument = nullptr;
    deleteArgument = nullptr;
    lastReference = last;
}

GuestLocale::Implementation MakeLocale(const GuestLocale::FacetVtable* vtable) {
    GuestLocale::Implementation locale{};
    locale.base.vtable = vtable;
    locale.base.references = 2;
    return locale;
}

bool Throws(void* pointer) {
    try {
        _Deletegloballocale_nid_postfix(pointer);
    } catch (const std::invalid_argument&) {
        return true;
    }
    return false;
}

}

int main() {
    Reset(true);
    GuestLocale::Implementation* none = nullptr;
    _Deletegloballocale_nid_postfix(&none);
    Require(releaseCalls == 0 && deleteCalls == 0, "a null stored locale makes no call");
    Require(none == nullptr, "a null stored locale stays null");

    auto shared = MakeLocale(&FakeVtable);
    GuestLocale::Implementation* sharedPointer = &shared;
    Reset(false);
    _Deletegloballocale_nid_postfix(&sharedPointer);
    Require(releaseCalls == 1, "a shared locale is released once");
    Require(releaseArgument == &shared.base, "release receives the locale facet");
    Require(deleteCalls == 0, "a locale with references left is not deleted");
    Require(sharedPointer == &shared, "the stored pointer is left unchanged");

    auto last = MakeLocale(&FakeVtable);
    GuestLocale::Implementation* lastPointer = &last;
    Reset(true);
    _Deletegloballocale_nid_postfix(&lastPointer);
    Require(releaseCalls == 1, "the last reference is released once");
    Require(deleteCalls == 1, "the last reference deletes the locale");
    Require(deleteArgument == &last.base, "delete receives the released facet");
    Require(lastPointer == &last, "the stored pointer is left unchanged after delete");

    Reset(true);
    Require(Throws(nullptr), "a null pointer throws");
    Require(releaseCalls == 0 && deleteCalls == 0, "a null pointer makes no call");

    auto noRelease = MakeLocale(&NoReleaseVtable);
    GuestLocale::Implementation* noReleasePointer = &noRelease;
    Reset(true);
    Require(Throws(&noReleasePointer), "an empty release slot throws");
    Require(deleteCalls == 0, "an empty release slot deletes nothing");

    auto noDelete = MakeLocale(&NoDeleteVtable);
    GuestLocale::Implementation* noDeletePointer = &noDelete;
    Reset(true);
    Require(Throws(&noDeletePointer), "an empty delete slot throws");
    Require(releaseCalls == 1, "an empty delete slot is found after the release");

    auto noVtable = MakeLocale(nullptr);
    GuestLocale::Implementation* noVtablePointer = &noVtable;
    Reset(true);
    Require(Throws(&noVtablePointer), "a missing vtable throws");

    if (failures != 0) return 1;
    std::puts("deletegloballocale: ok");
    return 0;
}
