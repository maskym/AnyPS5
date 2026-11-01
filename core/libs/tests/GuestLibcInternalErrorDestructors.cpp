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
unsigned allocations = 0;
unsigned releases = 0;

void Require(bool condition, const char* message) {
    if (condition) return;
    std::fprintf(stderr, "libc internal error destructors: %s\n", message);
    std::abort();
}

void* APS5_VABI allocate(std::size_t) {
    ++allocations;
    return storage.data();
}

void APS5_VABI release(void*) { ++releases; }

void* APS5_VABI allocateZeroed(std::size_t, std::size_t) { return allocate(0); }
void* APS5_VABI reallocate(void*, std::size_t) { return allocate(0); }
void* APS5_VABI align(std::size_t, std::size_t) { return allocate(0); }
void* APS5_VABI realign(void*, std::size_t, std::size_t) { return allocate(0); }
int APS5_VABI posixAlign(void** pointer, std::size_t, std::size_t) {
    *pointer = allocate(0);
    return 0;
}

LibcException::ExceptionObject* MakeError(const char* message) {
    auto* object = static_cast<LibcException::ExceptionObject*>(std::malloc(sizeof(LibcException::ExceptionObject)));
    Require(object != nullptr, "the host allocates the error object");
    _ZNSt13runtime_errorC1EPKc_nid_postfix(object, message);
    Require(object->vtable != nullptr && object->message != nullptr && std::strcmp(object->message, message) == 0, "the error object carries its message");
    return object;
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

    sceLibcInternalOverflowErrorDelete_nid_no_patch(MakeError("overflow"));
    sceLibcInternalUnderflowErrorDelete_nid_no_patch(MakeError("underflow"));
    sceLibcInternalUnderflowErrorDelete_nid_no_patch(MakeError("a message long enough to be kept on the heap rather than in place"));
    Require(allocations == 0 && releases == 0, "the deleting destructors use the host allocator, not the application heap");

    std::array<std::byte, 16> block{};
    block.fill(std::byte{0x5a});
    const auto copy = block;
    sceLibcInternalPlacementDelete_nid_no_patch(block.data(), storage.data());
    sceLibcInternalPlacementArrayDelete_nid_no_patch(block.data(), storage.data());
    sceLibcInternalPlacementDelete_nid_no_patch(nullptr, nullptr);
    sceLibcInternalPlacementArrayDelete_nid_no_patch(nullptr, nullptr);
    Require(block == copy && releases == 0 && allocations == 0, "the placement deletes touch nothing");

    std::puts("libc internal error destructor checks passed");
    return 0;
}
