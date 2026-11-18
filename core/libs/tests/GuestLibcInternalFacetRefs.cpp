#include "prx/libSceLibcInternal/include/InventoryExports.hpp"
#include "prx/libc/include/GuestLocale.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <stdexcept>
#include <thread>
#include <vector>

namespace {

void Require(bool condition, const char* message) {
    if (condition) return;
    std::fprintf(stderr, "libc internal locale::facet refs: %s\n", message);
    std::abort();
}

template <typename Function>
bool ThrowsInvalidArgument(Function function) {
    try {
        function();
    } catch (const std::invalid_argument&) {
        return true;
    }
    return false;
}

void TestSequence() {
    GuestLocale::Facet facet{nullptr, 1, 0xA5A5A5A5u};
    libSceLibcInternalInventory_076_nid_no_patch(&facet);
    Require(facet.references == 2, "_Incref did not increment the count");
    Require(libSceLibcInternalInventory_075_nid_no_patch(&facet) == nullptr, "_Decref to 1 returned the facet");
    Require(facet.references == 1, "_Decref did not decrement the count to 1");
    Require(libSceLibcInternalInventory_075_nid_no_patch(&facet) == &facet, "_Decref to 0 did not return the facet");
    Require(facet.references == 0, "_Decref did not decrement the count to 0");
    Require(libSceLibcInternalInventory_075_nid_no_patch(&facet) == nullptr, "_Decref from 0 returned the facet");
    Require(facet.references == std::numeric_limits<std::uint32_t>::max(), "_Decref from 0 did not wrap the 32-bit count");
    Require(facet.reserved == 0xA5A5A5A5u, "the upper count word was changed");
}

void TestNullFacet() {
    Require(ThrowsInvalidArgument([] { libSceLibcInternalInventory_076_nid_no_patch(nullptr); }), "_Incref accepted a null facet");
    Require(ThrowsInvalidArgument([] { libSceLibcInternalInventory_075_nid_no_patch(nullptr); }), "_Decref accepted a null facet");
}

void TestConcurrentIncrements() {
    constexpr int threadCount = 8;
    constexpr int incrementsPerThread = 10000;
    GuestLocale::Facet facet{nullptr, 0, 0};
    std::vector<std::thread> threads;
    for (int index = 0; index < threadCount; ++index) {
        threads.emplace_back([&facet] {
            for (int count = 0; count < incrementsPerThread; ++count) libSceLibcInternalInventory_076_nid_no_patch(&facet);
        });
    }
    for (std::thread& thread : threads) thread.join();
    Require(facet.references == threadCount * incrementsPerThread, "concurrent _Incref lost increments");
    int returned = 0;
    for (int count = 0; count < threadCount * incrementsPerThread; ++count) {
        GuestLocale::Facet* result = libSceLibcInternalInventory_075_nid_no_patch(&facet);
        if (result == nullptr) continue;
        Require(result == &facet, "_Decref returned another pointer");
        Require(count == threadCount * incrementsPerThread - 1, "_Decref returned the facet before the last release");
        ++returned;
    }
    Require(returned == 1, "_Decref did not return the facet exactly once");
    Require(facet.references == 0, "the count did not return to zero");
}

}

int main() {
    TestSequence();
    TestNullFacet();
    TestConcurrentIncrements();
    std::puts("libc internal locale::facet refs: ok");
    return 0;
}
