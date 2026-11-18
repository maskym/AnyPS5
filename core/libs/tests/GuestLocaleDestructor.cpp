#include "prx/libc/include/Api.hpp"
#include "prx/libSceLibcInternal/include/InventoryExports.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <source_location>
#include <stdexcept>

namespace {

int releaseCalls = 0;
int deleteCalls = 0;
bool releaseReturnsSelf = false;
GuestLocale::Facet* deleted = nullptr;

void APS5_VABI Destroy(GuestLocale::Facet*) {}

void APS5_VABI DeleteObject(GuestLocale::Facet* self) {
    ++deleteCalls;
    deleted = self;
}

void APS5_VABI Retain(GuestLocale::Facet*) {}

GuestLocale::Facet* APS5_VABI Release(GuestLocale::Facet* self) {
    ++releaseCalls;
    return releaseReturnsSelf ? self : nullptr;
}

const GuestLocale::FacetVtable testVtable{Destroy, DeleteObject, Retain, Release};

void Require(bool value, std::source_location location = std::source_location::current()) {
    if (value) return;
    std::fprintf(stderr, "locale destructor check failed at line %u\n", location.line());
    std::abort();
}

template<class Exception, class Function>
void RequireThrows(Function function) {
    bool thrown = false;
    try {
        function();
    } catch (const Exception&) {
        thrown = true;
    }
    Require(thrown);
}

void Reset(bool returnsSelf) {
    releaseCalls = 0;
    deleteCalls = 0;
    deleted = nullptr;
    releaseReturnsSelf = returnsSelf;
}

void CheckNullImplementation() {
    Reset(true);
    GuestLocaleFacade locale{nullptr};
    libSceLibcInternalInventory_083_nid_no_patch(&locale);
    Require(releaseCalls == 0 && deleteCalls == 0);
}

void CheckSharedImplementation() {
    Reset(false);
    GuestLocale::Implementation implementation{};
    implementation.base = {&testVtable, 2, 0};
    GuestLocaleFacade locale{&implementation};
    libSceLibcInternalInventory_083_nid_no_patch(&locale);
    Require(releaseCalls == 1 && deleteCalls == 0 && deleted == nullptr);
    Require(locale.implementation == &implementation);
}

void CheckLastReference() {
    Reset(true);
    GuestLocale::Implementation implementation{};
    implementation.base = {&testVtable, 1, 0};
    GuestLocaleFacade locale{&implementation};
    libSceLibcInternalInventory_083_nid_no_patch(&locale);
    Require(releaseCalls == 1 && deleteCalls == 1 && deleted == &implementation.base);
}

void CheckClassicLocale() {
    GuestLocaleFacade* classic = _ZNSt6locale7classicEv_nid_postfix();
    Require(classic != nullptr && classic->implementation != nullptr);
    const std::uint32_t before = classic->implementation->base.references;
    if (before != 1) classic->implementation->base.vtable->retain(&classic->implementation->base);
    GuestLocaleFacade copy{classic->implementation};
    libSceLibcInternalInventory_083_nid_no_patch(&copy);
    Require(classic->implementation->base.references == before);
    Require(copy.implementation == classic->implementation);
}

void CheckNullLocale() {
    RequireThrows<std::invalid_argument>([] { libSceLibcInternalInventory_083_nid_no_patch(nullptr); });
}

}

int main() {
    CheckNullImplementation();
    CheckSharedImplementation();
    CheckLastReference();
    CheckClassicLocale();
    CheckNullLocale();
    std::puts("locale destructor checks passed");
}
