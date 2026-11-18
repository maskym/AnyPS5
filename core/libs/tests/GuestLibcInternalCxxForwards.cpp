#include "prx/libSceLibcInternal/include/Api.hpp"
#include "prx/libc/include/Api.hpp"
#include "prx/libc/include/ApplicationHeap.hpp"
#include "prx/libc/include/general/VabiMacros.hpp"
#include <nid/NidCompute.hpp>
#include <array>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <system_error>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

extern "C" [[noreturn]] void APS5_VABI sceLibcInternalFilesystemErrorTwoPaths_nid_no_patch(const char* message, const GuestLocale::String* path1, const GuestLocale::String* path2, LibcException::ErrorCode code);

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

using FilesystemErrorTwoPaths = void(APS5_VABI*)(const char*, const GuestLocale::String*, const GuestLocale::String*, LibcException::ErrorCode);

struct Forward {
    const char* symbol;
    const char* nid;
};

constexpr std::array<Forward, 2> forwards {{
    {"_ZSt17iostream_categoryv", "V23qt24VPVs"},
    {"_ZNSt10filesystem18_Xfilesystem_errorEPKcRKNS_4pathES4_St10error_code", "Wut42WAe7Rw"},
}};

void* Resolve(const std::string& name) {
#ifdef _WIN32
    const HMODULE module = GetModuleHandleA("libSceLibcInternal.prx");
    Require(module != nullptr, "the libSceLibcInternal module is loaded");
    return reinterpret_cast<void*>(GetProcAddress(module, name.c_str()));
#else
    return dlsym(RTLD_DEFAULT, name.c_str());
#endif
}

std::string ModuleBytes() {
#ifdef _WIN32
    const HMODULE module = GetModuleHandleA("libSceLibcInternal.prx");
    Require(module != nullptr, "the libSceLibcInternal module is loaded");
    std::array<char, MAX_PATH> path {};
    const DWORD length = GetModuleFileNameA(module, path.data(), static_cast<DWORD>(path.size()));
    Require(length != 0 && length < path.size(), "the libSceLibcInternal module path is known");
    std::ifstream file(path.data(), std::ios::binary);
#else
    Dl_info owner {};
    Require(dladdr(reinterpret_cast<void*>(&sceLibcInternalIostreamCategory_nid_no_patch), &owner) != 0 && owner.dli_fname != nullptr, "the libSceLibcInternal module path is known");
    std::ifstream file(owner.dli_fname, std::ios::binary);
#endif
    Require(file.good(), "the libSceLibcInternal module file opens");
    return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

void CheckForwardsImportNoOwnNid() {
    const std::string bytes = ModuleBytes();
    for (const Forward& forward : forwards) {
        Require(Nid::ComputeNid(forward.symbol, "libc") == forward.nid, "the forward NID is the NID of its symbol");
        Require(Resolve(std::string(forward.nid) + "_nid_no_patch_cut") != nullptr, "libSceLibcInternal exports the forward NID");
        Require(bytes.find(std::string(forward.symbol) + "_nid_postfix") == std::string::npos, "the forward does not import the NID it exports");
    }
}

GuestLocale::String ShortString(const char* text) {
    GuestLocale::String string {};
    string.capacity = 15;
    string.size = std::strlen(text);
    std::memcpy(string.buffer, text, string.size + 1);
    return string;
}

template<typename Call>
std::string InvalidArgument(Call call) {
    try { call(); } catch (const std::invalid_argument& error) { return error.what(); }
    return {};
}

void CheckFilesystemErrorTwoPaths() {
    const auto* generic = _ZSt16generic_categoryv_nid_postfix();
    const GuestLocale::String a = ShortString("a");
    const GuestLocale::String b = ShortString("b");
    void* const symbol = Resolve("Wut42WAe7Rw_nid_no_patch_cut");
    Require(symbol != nullptr, "the two-path filesystem error export resolves");
    const auto exported = reinterpret_cast<FilesystemErrorTwoPaths>(symbol);
    const std::string expected = InvalidArgument([&] { FilesystemErrorTwoPaths_nid_no_patch("x", nullptr, &b, {0, 2, generic}); });
    Require(!expected.empty(), "a null first path throws invalid_argument");
    Require(InvalidArgument([&] { _ZNSt10filesystem18_Xfilesystem_errorEPKcRKNS_4pathES4_St10error_code_nid_postfix("x", nullptr, &b, {0, 2, generic}); }) == expected, "the libc export calls the shared body");
    Require(InvalidArgument([&] { sceLibcInternalFilesystemErrorTwoPaths_nid_no_patch("x", nullptr, &b, {0, 2, generic}); }) == expected, "the forward calls the shared body");
    Require(InvalidArgument([&] { exported("x", nullptr, &b, {0, 2, generic}); }) == expected, "the Wut42WAe7Rw export reaches the shared body");
    Require(!InvalidArgument([&] { sceLibcInternalFilesystemErrorTwoPaths_nid_no_patch(nullptr, &a, &b, {0, 2, generic}); }).empty(), "a null message throws invalid_argument");
    Require(!InvalidArgument([&] { sceLibcInternalFilesystemErrorTwoPaths_nid_no_patch("x", &a, nullptr, {0, 2, generic}); }).empty(), "a null second path throws invalid_argument");
    Require(!InvalidArgument([&] { sceLibcInternalFilesystemErrorTwoPaths_nid_no_patch("x", &a, &b, {0, 2, nullptr}); }).empty(), "a null category throws invalid_argument");
    Require(!InvalidArgument([&] { exported("x", &a, &b, {0, 2, nullptr}); }).empty(), "the Wut42WAe7Rw export checks the category");
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

    const std::error_category* const category = sceLibcInternalIostreamCategory_nid_no_patch();
    Require(category != nullptr, "the iostream category exists");
    Require(category == _ZSt17iostream_categoryv_nid_postfix(), "the iostream category is the libc object");
    Require(category == IostreamCategory_nid_no_patch(), "the forward and the libc export share one body");
    Require(category == sceLibcInternalIostreamCategory_nid_no_patch(), "the iostream category is a singleton");
    Require(std::strcmp(category->name(), "iostream") == 0, "the category is named iostream");

    CheckFilesystemErrorTwoPaths();
    CheckForwardsImportNoOwnNid();

    std::puts("libc internal C++ runtime forward checks passed");
    return 0;
}
