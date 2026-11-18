#include "prx/libkernel/include/Api.hpp"
#include "prx/libc/include/Api.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>

namespace {

constexpr int GetStatusFlags = 3;
constexpr int SetStatusFlags = 4;
constexpr int ReadWrite = 2;
constexpr int Nonblocking = 4;
constexpr int Append = 8;
constexpr int Asynchronous = 0x40;
constexpr int Ebadf = 9;
constexpr int Enotty = 25;

void Require(bool value, const char* message) {
    if (value) return;
    std::fprintf(stderr, "%s\n", message);
    std::abort();
}

void RequireFailure(int result, int error, const char* message) {
    Require(result == -1, message);
    Require(*__error_nid_postfix() == error, message);
}

void CheckStatusFlags(int descriptor) {
    Require(fcntl_nid_postfix(descriptor, GetStatusFlags) == ReadWrite, "F_GETFL must report O_RDWR on a new descriptor");
    RequireFailure(fcntl_nid_postfix(descriptor, SetStatusFlags, 0), Enotty, "F_SETFL 0 must fail with ENOTTY");
    Require(fcntl_nid_postfix(descriptor, GetStatusFlags) == ReadWrite, "F_GETFL must report O_RDWR after F_SETFL 0");
    RequireFailure(fcntl_nid_postfix(descriptor, SetStatusFlags, Nonblocking), Enotty, "F_SETFL O_NONBLOCK must fail with ENOTTY");
    Require(fcntl_nid_postfix(descriptor, GetStatusFlags) == (ReadWrite | Nonblocking), "F_SETFL must record O_NONBLOCK before failing");
    RequireFailure(fcntl_nid_postfix(descriptor, SetStatusFlags, Asynchronous | Append), Enotty, "F_SETFL O_ASYNC | O_APPEND must fail with ENOTTY");
    Require(fcntl_nid_postfix(descriptor, GetStatusFlags) == (ReadWrite | Asynchronous | Append), "F_SETFL must replace the recorded flags");
    RequireFailure(fcntl_nid_postfix(descriptor, SetStatusFlags, ReadWrite), Enotty, "F_SETFL O_RDWR must fail with ENOTTY");
    Require(fcntl_nid_postfix(descriptor, GetStatusFlags) == ReadWrite, "F_SETFL must drop the access mode and clear the recorded flags");
}

}

int main() {
    const int queue = kqueue_nid_postfix();
    Require(queue >= 0, "kqueue must succeed");
    CheckStatusFlags(queue);
    Require(close_nid_postfix(queue) == 0, "close of the kqueue must succeed");

    std::intptr_t semaphore = -1;
    Require(ksem_init_nid_postfix(&semaphore, 0) == 0 && semaphore >= 0, "ksem_init must succeed");
    CheckStatusFlags(static_cast<int>(semaphore));
    Require(ksem_destroy_nid_postfix(semaphore) == 0, "ksem_destroy must succeed");

    RequireFailure(fcntl_nid_postfix(9999, SetStatusFlags, Nonblocking), Ebadf, "F_SETFL on a closed descriptor must fail with EBADF");
    return 0;
}
