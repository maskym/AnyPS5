#include <cstdio>
#include <cstring>
#include <cstdint>
#include <filesystem>
#include <string>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <dlfcn.h>
#endif

struct SharedExport {
    const char* name;
    const char* nid;
};

static const SharedExport SharedExports[] = {
    {"RSA_new", "kADEz2eAE7c"},
    {"RSA_free", "5zpup0URQoc"},
    {"d2i_RSA_PUBKEY", "PFxpQ6lkQP0"},
    {"EVP_PKEY_get1_RSA", "5RBpaKVkWRQ"},
    {"PEM_read_RSAPrivateKey", "psMmvmdPKds"},
};

#ifdef _WIN32
static std::filesystem::path ModulePath(HMODULE module) {
    std::wstring buffer(1024, L'\0');
    for (;;) {
        const auto size = GetModuleFileNameW(module, buffer.data(), static_cast<DWORD>(buffer.size()));
        if (size == 0) return {};
        if (size < buffer.size()) {
            buffer.resize(size);
            return std::filesystem::canonical(buffer);
        }
        if (buffer.size() >= 32768) return {};
        buffer.resize(buffer.size() * 2);
    }
}

static std::filesystem::path Owner(const void* address) {
    HMODULE module = nullptr;
    if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                            static_cast<LPCWSTR>(address), &module))
        return {};
    return ModulePath(module);
}

static std::filesystem::path DefiningFile(HMODULE handle, const char* nid) {
    const auto address = reinterpret_cast<const unsigned char*>(GetProcAddress(handle, nid));
    if (address == nullptr) return {};
    if (address[0] == 0xFF && address[1] == 0x25) {
        std::int32_t displacement = 0;
        std::memcpy(&displacement, address + 2, sizeof(displacement));
        const void* slot = address + 6 + displacement;
        void* target = nullptr;
        std::memcpy(&target, slot, sizeof(target));
        return Owner(target);
    }
    return Owner(address);
}
#else
static std::filesystem::path DefiningFile(void* handle, const char* nid) {
    void* address = dlsym(handle, nid);
    if (address == nullptr) return {};
    Dl_info info{};
    if (dladdr(address, &info) == 0 || info.dli_fname == nullptr) return {};
    return std::filesystem::canonical(std::filesystem::path(info.dli_fname));
}
#endif

int main(int argc, char** argv) {
    if (argc != 2) {
        std::fprintf(stderr, "usage: %s <patched libraries directory>\n", argv[0]);
        return 2;
    }
    const std::filesystem::path directory = std::filesystem::canonical(argv[1]);
#ifdef _WIN32
    SetErrorMode(SEM_FAILCRITICALERRORS);
    HMODULE libreSsl3 = LoadLibraryExW((directory / "libSceLibreSsl3.prx").c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
    if (libreSsl3 == nullptr) {
        std::fprintf(stderr, "libSceLibreSsl3.prx failed to load: %lu\n", GetLastError());
        return 1;
    }
    HMODULE libreSsl = LoadLibraryExW((directory / "libSceLibreSsl.prx").c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
    if (libreSsl == nullptr) {
        std::fprintf(stderr, "libSceLibreSsl.prx failed to load: %lu\n", GetLastError());
        return 1;
    }
#else
    void* libreSsl3 = dlopen((directory / "libSceLibreSsl3.prx").c_str(), RTLD_NOW | RTLD_LOCAL);
    void* libreSsl = dlopen((directory / "libSceLibreSsl.prx").c_str(), RTLD_NOW | RTLD_LOCAL);
    if (libreSsl3 == nullptr || libreSsl == nullptr) {
        std::fprintf(stderr, "%s\n", dlerror());
        return 1;
    }
#endif
    const std::filesystem::path libreSslFile = std::filesystem::canonical(directory / "libSceLibreSsl.prx");
    const std::filesystem::path libreSsl3File = std::filesystem::canonical(directory / "libSceLibreSsl3.prx");
    int failed = 0;
    for (const auto& entry : SharedExports) {
        const std::filesystem::path own = DefiningFile(libreSsl, entry.nid);
        if (own.empty() || own != libreSslFile) {
            std::fprintf(stderr, "%s (%s) is not exported by libSceLibreSsl\n", entry.name, entry.nid);
            ++failed;
        }
        const std::filesystem::path bound = DefiningFile(libreSsl3, entry.nid);
        if (bound.empty() || bound != libreSsl3File) {
            std::fprintf(stderr, "%s (%s) imported from libSceLibreSsl3 binds to %s\n", entry.name, entry.nid,
                         bound.empty() ? "nothing" : bound.string().c_str());
            ++failed;
        }
    }
    return failed == 0 ? 0 : 1;
}
