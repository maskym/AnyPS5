#include "prx/libc/include/Api.hpp"
#include "prx/libSceLibcInternal/include/InventoryExports6.hpp"
#include <cstdio>
#include <cstdlib>

namespace {

void Require(bool condition, const char* message) {
    if (condition) return;
    std::fprintf(stderr, "libc internal error category destructors: %s\n", message);
    std::abort();
}

}

int main() {
    const LibcException::CategoryObject* future = _ZSt15future_categoryv_nid_postfix();
    const void* const futureVtable = future->vtable;
    Require(futureVtable != nullptr, "the future category has a vtable");

    LibcException::CategoryObject copy{futureVtable};
    _ZNSt22_Future_error_categoryD1Ev_nid_postfix(&copy);
    Require(copy.vtable == futureVtable, "the destructor leaves the object's vtable in place");
    Require(future->vtable == futureVtable, "the shared future category is untouched");
    Require(_ZSt15future_categoryv_nid_postfix() == future, "future_category still returns the same object");

    std::puts("libc internal error category destructor checks passed");
    return 0;
}
