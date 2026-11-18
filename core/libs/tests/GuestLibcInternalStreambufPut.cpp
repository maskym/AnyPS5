#include "prx/libc/include/GuestLocale.hpp"
#include "prx/libc/include/general/VabiMacros.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace {

void Require(bool condition, const char* message) {
    if (condition) return;
    std::fprintf(stderr, "libc internal streambuf put: %s\n", message);
    std::exit(1);
}

#ifdef _WIN32
HMODULE g_library = nullptr;
#else
void* g_library = nullptr;
#endif

template<typename TFunction>
TFunction Export(const char* nid) {
    const std::string name = std::string(nid) + "_nid_no_patch_cut";
#ifdef _WIN32
    auto* address = reinterpret_cast<void*>(GetProcAddress(g_library, name.c_str()));
#else
    void* address = dlsym(g_library, name.c_str());
#endif
    Require(address != nullptr, "missing export");
    return reinterpret_cast<TFunction>(address);
}

using NarrowXsputn = int (APS5_VABI*)(GuestLocale::Streambuf*, const char*, int);
using WideXsputn = int (APS5_VABI*)(GuestLocale::Streambuf*, const std::uint16_t*, int);

struct TestStreambuf {
    GuestLocale::Streambuf buffer;
    std::vector<int> offered;
    std::size_t limit;
};

int APS5_VABI TestOverflow(GuestLocale::Streambuf* self, int element) {
    auto* stream = reinterpret_cast<TestStreambuf*>(self);
    if (stream->offered.size() >= stream->limit) {
        stream->offered.push_back(-2);
        return -1;
    }
    stream->offered.push_back(element);
    return element;
}

const GuestLocale::StreambufVtable g_streambufVtable{{}, TestOverflow};

struct PutArea {
    char* next;
    char* end;
};

TestStreambuf MakeStream(PutArea& area, std::size_t limit) {
    return TestStreambuf{{&g_streambufVtable, {}, &area.next, {}, &area.end}, {}, limit};
}

void CheckNarrowArea(NarrowXsputn xsputn) {
    std::array<char, 8> storage{};
    PutArea area{storage.data(), storage.data() + 4};
    TestStreambuf stream = MakeStream(area, 2);
    Require(xsputn(&stream.buffer, "abcdefgh", 8) == 6, "narrow area count");
    Require(std::memcmp(storage.data(), "abcd", 4) == 0, "narrow area contents");
    Require(storage[4] == '\0', "narrow area bound");
    Require(area.next == storage.data() + 4, "narrow area advance");
    Require(stream.offered == std::vector<int>{'e', 'f', -2}, "narrow overflow sequence");
}

void CheckNarrowNullArea(NarrowXsputn xsputn) {
    PutArea area{nullptr, nullptr};
    TestStreambuf stream = MakeStream(area, 16);
    Require(xsputn(&stream.buffer, "xyz", 3) == 3, "null area count");
    Require(stream.offered == std::vector<int>{'x', 'y', 'z'}, "null area overflow");
    Require(area.next == nullptr, "null area untouched");
}

void CheckNonPositiveCount(NarrowXsputn xsputn, WideXsputn wideXsputn) {
    std::array<char, 4> storage{};
    PutArea area{storage.data(), storage.data() + 4};
    TestStreambuf stream = MakeStream(area, 16);
    Require(xsputn(&stream.buffer, "abc", 0) == 0, "zero count");
    Require(xsputn(&stream.buffer, "abc", -3) == 0, "negative count");
    Require(wideXsputn(&stream.buffer, nullptr, 0) == 0, "wide zero count");
    Require(stream.offered.empty(), "non-positive count overflow");
    Require(area.next == storage.data(), "non-positive count advance");
}

void CheckHighByte(NarrowXsputn xsputn) {
    PutArea area{nullptr, nullptr};
    TestStreambuf stream = MakeStream(area, 16);
    const char bytes[] = {static_cast<char>(0xFF), static_cast<char>(0x80)};
    Require(xsputn(&stream.buffer, bytes, 2) == 2, "high byte count");
    Require(stream.offered == std::vector<int>{255, 128}, "high byte widened unsigned");
}

void CheckWideArea(WideXsputn xsputn) {
    std::array<std::uint16_t, 4> storage{};
    char* base = reinterpret_cast<char*>(storage.data());
    PutArea area{base, base + 3 * sizeof(std::uint16_t)};
    TestStreambuf stream = MakeStream(area, 1);
    const std::uint16_t text[] = {u'w', u'x', u'y', u'z', u'1', u'2'};
    Require(xsputn(&stream.buffer, text, 6) == 4, "wide area count");
    Require(storage[0] == u'w' && storage[1] == u'x' && storage[2] == u'y' && storage[3] == 0, "wide area contents");
    Require(area.next == base + 3 * sizeof(std::uint16_t), "wide area advance");
    Require(stream.offered == std::vector<int>{u'z', -2}, "wide overflow sequence");
}

void CheckWideOddArea(WideXsputn xsputn) {
    std::array<std::uint16_t, 4> storage{};
    char* base = reinterpret_cast<char*>(storage.data());
    PutArea area{base, base + 5};
    TestStreambuf stream = MakeStream(area, 16);
    const std::uint16_t text[] = {0x1234, 0xFFFF, 0x00E9};
    Require(xsputn(&stream.buffer, text, 3) == 3, "odd area count");
    Require(storage[0] == 0x1234 && storage[1] == 0xFFFF && storage[2] == 0, "odd area contents");
    Require(area.next == base + 2 * sizeof(std::uint16_t), "odd area advance");
    Require(stream.offered == std::vector<int>{0x00E9}, "odd area overflow");
}

void CheckWideHighElement(WideXsputn xsputn) {
    PutArea area{nullptr, nullptr};
    TestStreambuf stream = MakeStream(area, 16);
    const std::uint16_t text[] = {0xFFFF, 0x8000};
    Require(xsputn(&stream.buffer, text, 2) == 2, "wide high count");
    Require(stream.offered == std::vector<int>{0xFFFF, 0x8000}, "wide high widened unsigned");
}

}

int main(int argc, char** argv) {
#ifdef _WIN32
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
#endif
    Require(argc == 2, "expected the libSceLibcInternal path");
#ifdef _WIN32
    g_library = LoadLibraryA(argv[1]);
#else
    g_library = dlopen(argv[1], RTLD_NOW | RTLD_LOCAL);
#endif
    Require(g_library != nullptr, "load libSceLibcInternal");
    const auto xsputn = Export<NarrowXsputn>("sXaxl1QGorg");
    const auto wideXsputn = Export<WideXsputn>("041c37QfoUc");
    CheckNarrowArea(xsputn);
    CheckNarrowNullArea(xsputn);
    CheckNonPositiveCount(xsputn, wideXsputn);
    CheckHighByte(xsputn);
    CheckWideArea(wideXsputn);
    CheckWideOddArea(wideXsputn);
    CheckWideHighElement(wideXsputn);
    std::puts("libc internal streambuf put: ok");
    return 0;
}
