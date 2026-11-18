#include "prx/libc/include/Api.hpp"
#include "prx/libc/include/general/VabiMacros.hpp"
#include "prx/libc/include/GuestLocale.hpp"
#include "prx/libc/include/ApplicationHeap.hpp"
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

struct TypeRecord { const void* vtable; const char* name; const TypeRecord* base; };

static void Require(bool value, int code) {
    if (value) return;
    std::fprintf(stderr, "ios_base::failure constructor check %d failed\n", code);
    std::exit(code);
}

static const char* TypeName(const LibcException::SystemErrorObject& object) {
    const auto* vtable = static_cast<const void* const*>(object.base.vtable);
    return static_cast<const TypeRecord*>(vtable[-1])->name;
}

static GuestLocale::String* APS5_VABI TestCategoryMessage(GuestLocale::String* result, const LibcException::CategoryObject*, int) {
    result->reserved = 0;
    result->size = 16;
    result->capacity = 31;
    result->pointer = static_cast<char*>(GuestLocale::AllocateStringStorage(17));
    std::memcpy(result->pointer, "Invalid argument", 17);
    return result;
}
static const void* const TestCategoryVtable[] = {nullptr, nullptr, nullptr, reinterpret_cast<const void*>(TestCategoryMessage)};
static const LibcException::CategoryObject TestCategory {TestCategoryVtable};

static void CheckMessageConstructor() {
    const LibcException::ErrorCode code {0, 22, &TestCategory};
    LibcException::SystemErrorObject failure {};
    _ZNSt8ios_base7failureC1EPKcRKSt10error_code_nid_postfix(&failure, "stream", &code);
    Require(std::strcmp(TypeName(failure), "NSt8ios_base7failureE") == 0, 1);
    const char* what = _ZNKSt8ios_base7failure4whatEv_nid_postfix(&failure.base);
    Require(what && std::strcmp(what, "stream: Invalid argument") == 0, 2);
    Require(failure.code.reserved == 0 && failure.code.value == 22 && failure.code.category == &TestCategory, 3);
    _ZNSt8ios_base7failureD1Ev_nid_postfix(&failure.base);
    Require(failure.base.message == nullptr, 4);

    LibcException::SystemErrorObject empty {};
    _ZNSt8ios_base7failureC2EPKcRKSt10error_code_nid_postfix(&empty, "", &code);
    Require(std::strcmp(TypeName(empty), "NSt8ios_base7failureE") == 0, 5);
    const char* emptyWhat = _ZNKSt8ios_base7failure4whatEv_nid_postfix(&empty.base);
    Require(emptyWhat && std::strcmp(emptyWhat, "Invalid argument") == 0, 6);
    Require(empty.code.value == 22 && empty.code.category == &TestCategory, 7);
    _ZNSt8ios_base7failureD2Ev_nid_postfix(&empty.base);
}

static void CheckCopyConstructor() {
    const LibcException::ErrorCode code {0, 22, &TestCategory};
    LibcException::SystemErrorObject source {};
    _ZNSt8ios_base7failureC1EPKcRKSt10error_code_nid_postfix(&source, "stream", &code);
    LibcException::SystemErrorObject copy {};
    _ZNSt8ios_base7failureC1ERKS0__nid_postfix(&copy, &source);
    Require(std::strcmp(TypeName(copy), "NSt8ios_base7failureE") == 0, 10);
    Require(copy.base.message == source.base.message, 11);
    Require(copy.code.reserved == 0 && copy.code.value == 22 && copy.code.category == &TestCategory, 12);
    _ZNSt8ios_base7failureD1Ev_nid_postfix(&source.base);
    Require(source.base.message == nullptr, 13);
    const char* what = _ZNKSt8ios_base7failure4whatEv_nid_postfix(&copy.base);
    Require(what && std::strcmp(what, "stream: Invalid argument") == 0, 14);
    _ZNSt8ios_base7failureD1Ev_nid_postfix(&copy.base);
    Require(copy.base.message == nullptr, 15);
}

static void CheckInvalidArguments() {
    bool nullCode = false;
    try {
        LibcException::SystemErrorObject failure {};
        _ZNSt8ios_base7failureC1EPKcRKSt10error_code_nid_postfix(&failure, "stream", nullptr);
    } catch (const std::invalid_argument&) {
        nullCode = true;
    }
    Require(nullCode, 20);
    bool nullCategory = false;
    try {
        const LibcException::ErrorCode code {0, 22, nullptr};
        LibcException::SystemErrorObject failure {};
        _ZNSt8ios_base7failureC2EPKcRKSt10error_code_nid_postfix(&failure, "stream", &code);
    } catch (const std::invalid_argument&) {
        nullCategory = true;
    }
    Require(nullCategory, 21);
}

int main() {
    const std::array<void*, 10> api {};
    ApplicationHeapRegister_nid_no_patch(api.data());
    CheckMessageConstructor();
    CheckCopyConstructor();
    CheckInvalidArguments();
    std::puts("ios_base::failure constructors passed");
    return 0;
}
