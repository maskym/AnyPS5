#include "prx/libSceLibcInternal/include/Api.hpp"
#include "prx/libc/include/Api.hpp"
#include "prx/libc/include/ApplicationHeap.hpp"
#include "prx/libc/include/general/VabiMacros.hpp"
#include <array>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {

alignas(64) std::array<std::byte, 64> storage{};
unsigned releases = 0;
void* lastReleased = nullptr;

void Require(bool condition, const char* message) {
    if (condition) return;
    std::fprintf(stderr, "libc internal C++ runtime forwards: %s\n", message);
    std::abort();
}

void* APS5_VABI allocate(std::size_t) { return storage.data(); }

void APS5_VABI release(void* pointer) {
    ++releases;
    lastReleased = pointer;
}

void* APS5_VABI allocateZeroed(std::size_t, std::size_t) { return storage.data(); }
void* APS5_VABI reallocate(void*, std::size_t) { return storage.data(); }
void* APS5_VABI align(std::size_t, std::size_t) { return storage.data(); }
void* APS5_VABI realign(void*, std::size_t, std::size_t) { return storage.data(); }
int APS5_VABI posixAlign(void** pointer, std::size_t, std::size_t) {
    *pointer = storage.data();
    return 0;
}

void Reset() {
    releases = 0;
    lastReleased = nullptr;
}

struct TypeLayout { const void* const* vtable; const char* name; const TypeLayout* base; };

void RequireMessage(GuestLocale::String* (APS5_VABI *message)(GuestLocale::String*, const LibcException::CategoryObject*, int),
    const LibcException::CategoryObject* category, int value, const char* expected, const char* description) {
    GuestLocale::String text {};
    Require(message(&text, category, value) == &text, description);
    const char* data = text.capacity > 15 ? text.pointer : text.buffer;
    Require(text.size == std::strlen(expected) && std::strcmp(data, expected) == 0, description);
    GuestLocale::ReleaseStringStorage(text);
}

}

int main() {
    std::array<void*, 10> api{};
    api[0] = reinterpret_cast<void*>(&allocate);
    api[1] = reinterpret_cast<void*>(&release);
    api[2] = reinterpret_cast<void*>(&allocateZeroed);
    api[3] = reinterpret_cast<void*>(&reallocate);
    api[4] = reinterpret_cast<void*>(&align);
    api[5] = reinterpret_cast<void*>(&realign);
    api[6] = reinterpret_cast<void*>(&posixAlign);
    ApplicationHeapRegister_nid_no_patch(api.data());

    const int nothrowTag = 0;
    Reset();
    sceLibcInternalSizedNothrowDelete_nid_no_patch(storage.data(), 24, &nothrowTag);
    Require(releases == 1 && lastReleased == storage.data(), "the sized nothrow delete releases the block");

    Reset();
    sceLibcInternalSizedNothrowArrayDelete_nid_no_patch(storage.data(), 64, nullptr);
    Require(releases == 1 && lastReleased == storage.data(), "the sized nothrow array delete releases the block");

    Reset();
    sceLibcInternalSizedNothrowDelete_nid_no_patch(nullptr, 24, &nothrowTag);
    sceLibcInternalSizedNothrowArrayDelete_nid_no_patch(nullptr, 64, &nothrowTag);
    Require(releases == 0, "deleting null releases nothing");

    const LibcException::CategoryObject* const category = sceLibcInternalIostreamCategory_nid_no_patch();
    Require(category != nullptr, "the iostream category exists");
    Require(category == _ZSt17iostream_categoryv_nid_postfix(), "the iostream category is the libc object");
    Require(category == IostreamCategory_nid_no_patch(), "the forward and the libc export share one body");
    Require(category == sceLibcInternalIostreamCategory_nid_no_patch(), "the iostream category is a singleton");
    const auto* const vtable = static_cast<const void* const*>(category->vtable);
    const auto* const type = static_cast<const TypeLayout*>(vtable[-1]);
    const auto* const genericType = static_cast<const TypeLayout*>(static_cast<const void* const*>(_ZSt16generic_categoryv_nid_postfix()->vtable)[-1]);
    Require(std::strcmp(type->name, "St24_Iostream_error_category") == 0, "the category has the _Iostream_error_category typeinfo");
    Require(type->base == genericType, "_Iostream_error_category derives from _Generic_error_category");
    using Name = const char* (APS5_VABI *)(const LibcException::CategoryObject*);
    const auto name = reinterpret_cast<Name>(const_cast<void*>(vtable[2]));
    Require(std::strcmp(name(category), "iostream") == 0, "the category is named iostream");
    using Message = GuestLocale::String* (APS5_VABI *)(GuestLocale::String*, const LibcException::CategoryObject*, int);
    const auto message = reinterpret_cast<Message>(const_cast<void*>(vtable[3]));
    RequireMessage(message, category, 1, "iostream stream error", "io_errc::stream has the iostream message");
    RequireMessage(message, category, 22, "invalid argument", "other values have the generic message");
    RequireMessage(message, category, 9999, "unknown error", "an unknown value has the generic fallback");
    RequireMessage(sceLibcInternalIostreamCategoryMessage_nid_no_patch, category, 1, "iostream stream error", "the message forward matches the vtable");
    RequireMessage(sceLibcInternalIostreamCategoryMessage_nid_no_patch, category, 22, "invalid argument", "the message forward falls back to the generic message");

    std::puts("libc internal C++ runtime forward checks passed");
    return 0;
}
