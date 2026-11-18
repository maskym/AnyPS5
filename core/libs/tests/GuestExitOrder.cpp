#include "prx/libc/include/Api.hpp"
#include "prx/libc/include/Shutdown.hpp"
#include "prx/libc/include/general/VabiMacros.hpp"
#include <cstdio>
#include <cstdlib>
#include <string>

namespace {

void Record(char value) {
    std::fputc(value, stdout);
    std::fflush(stdout);
}

void APS5_VABI RecordGuestExit(void*) { Record('G'); }

void HostHandler() { Record('H'); }

void APS5_VABI AtexitHandler() { Record('A'); }

}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::fputs("usage: guest_exit_order_tests <case>\n", stderr);
        return 1;
    }
    const std::string mode = argv[1];
    if (mode == "guest-before-host") {
        __cxa_atexit_nid_postfix(RecordGuestExit, nullptr, nullptr);
        std::atexit(HostHandler);
    } else if (mode == "reverse-registration") {
        __cxa_atexit_nid_postfix(RecordGuestExit, nullptr, nullptr);
        atexit_nid_postfix(AtexitHandler);
    } else {
        std::fputs("unknown case\n", stderr);
        return 1;
    }
    std::fputs("order:", stdout);
    std::fflush(stdout);
    LibcExit_nid_no_patch(0);
}
