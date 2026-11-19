#include "prx/libc/include/Api.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C" void CallWithCleanup(void (*function)(), int* count);
extern "C" void CallAroundCleanup(void (*before)(), void (*within)(), int* count);

int cleanups;

struct Counted { ~Counted() { ++cleanups; } };

[[gnu::noinline]] void Throw() { throw 42; }
[[gnu::noinline]] void Nothing() {}
[[gnu::noinline]] void ThrowThroughCleanup() { CallWithCleanup(Throw, &cleanups); }

bool Caught(void (*function)(), int expectedCleanups) {
    cleanups = 0;
    try {
        function();
    } catch (int value) {
        return value == 42 && cleanups == expectedCleanups;
    }
    return false;
}

[[noreturn]] void TerminateWithoutCleanup() {
    cleanups = 0;
    _ZSt13set_terminatePFvvE_nid_postfix([] { std::_Exit(cleanups == 0 ? 0 : 62); });
    CallWithCleanup(Throw, &cleanups);
    std::_Exit(63);
}

int main(int argc, char** argv) {
    if (argc > 1 && std::strcmp(argv[1], "terminate") == 0) TerminateWithoutCleanup();
    if (!Caught([] { CallWithCleanup(Throw, &cleanups); }, 1)) return 1;
    if (!Caught([] { CallWithCleanup(ThrowThroughCleanup, &cleanups); }, 2)) return 2;
    if (!Caught([] { CallAroundCleanup(Throw, Nothing, &cleanups); }, 0)) return 3;
    if (!Caught([] { CallAroundCleanup(Nothing, Throw, &cleanups); }, 1)) return 4;
    if (!Caught([] { Counted counted; CallWithCleanup(Throw, &cleanups); }, 2)) return 5;
    std::puts("exception C cleanup tests passed");
    return 0;
}
