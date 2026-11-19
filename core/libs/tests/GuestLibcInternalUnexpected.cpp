#include "prx/libSceLibcInternal/include/Api.hpp"
#include "prx/libc/include/Api.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <stdexcept>
#include <string>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/wait.h>
#include <unistd.h>
#endif
#ifdef __APPLE__
#include <mach-o/dyld.h>
#include <vector>
#endif

namespace {

void Require(bool condition, const char* message) {
    if (condition) return;
    std::fprintf(stderr, "libc internal unexpected: %s\n", message);
    std::abort();
}

bool customHandler1Called = false;
void CustomHandler1() {
    customHandler1Called = true;
}

bool customHandler2Called = false;
void CustomHandler2() {
    customHandler2Called = true;
}

struct TestUnexpectedException {};

void ThrowingHandler() {
    throw TestUnexpectedException{};
}

void RuntimeErrorHandler() {
    throw std::runtime_error("unexpected exception message");
}

void CheckUnexpectedHandlerLifecycle() {
    LibcUnexpectedHandler initial = _ZSt14get_unexpectedv_nid_postfix();
    Require(initial != nullptr, "initial unexpected handler is not null");

    LibcUnexpectedHandler prev1 = _ZSt14set_unexpectedPFvvE_nid_postfix(&CustomHandler1);
    Require(prev1 == initial, "setting custom handler returns initial handler");
    Require(_ZSt14get_unexpectedv_nid_postfix() == &CustomHandler1, "get_unexpected returns CustomHandler1");

    LibcUnexpectedHandler prev2 = _ZSt14set_unexpectedPFvvE_nid_postfix(&CustomHandler2);
    Require(prev2 == &CustomHandler1, "setting second handler returns CustomHandler1");
    Require(_ZSt14get_unexpectedv_nid_postfix() == &CustomHandler2, "get_unexpected returns CustomHandler2");

    LibcUnexpectedHandler prevNull = _ZSt14set_unexpectedPFvvE_nid_postfix(nullptr);
    Require(prevNull == &CustomHandler2, "setting null handler returns CustomHandler2");
    LibcUnexpectedHandler restored = _ZSt14get_unexpectedv_nid_postfix();
    Require(restored != nullptr && restored == initial, "setting null handler restores initial default handler");
}

void CheckUnexpectedInvocation() {
    _ZSt14set_unexpectedPFvvE_nid_postfix(&ThrowingHandler);
    Require(_ZSt14get_unexpectedv_nid_postfix() == &ThrowingHandler, "throwing handler installed");

    bool caughtCustom = false;
    try {
        _ZSt10unexpectedv_nid_postfix();
    } catch (const TestUnexpectedException&) {
        caughtCustom = true;
    }
    Require(caughtCustom, "throwing handler exception propagates through unexpected");

    _ZSt14set_unexpectedPFvvE_nid_postfix(&RuntimeErrorHandler);
    bool caughtRuntime = false;
    try {
        _ZSt10unexpectedv_nid_postfix();
    } catch (const std::runtime_error& error) {
        caughtRuntime = std::strcmp(error.what(), "unexpected exception message") == 0;
    }
    Require(caughtRuntime, "runtime error propagates with expected message");

    _ZSt14set_unexpectedPFvvE_nid_postfix(nullptr);
}

std::filesystem::path OwnExecutable() {
#ifdef _WIN32
    std::wstring executable(32768, L'\0');
    executable.resize(GetModuleFileNameW(nullptr, executable.data(), static_cast<DWORD>(executable.size())));
    return executable;
#elif defined(__APPLE__)
    std::uint32_t size = 0;
    _NSGetExecutablePath(nullptr, &size);
    std::vector<char> executable(size);
    Require(_NSGetExecutablePath(executable.data(), &size) == 0, "executable path lookup failed");
    return std::filesystem::canonical(executable.data());
#else
    return std::filesystem::read_symlink("/proc/self/exe");
#endif
}

int RunChild(const std::filesystem::path& self, const char* mode) {
#ifdef _WIN32
    std::wstring commandLine = L"\"" + self.wstring() + L"\" " + std::wstring(mode, mode + std::strlen(mode));
    STARTUPINFOW startup{};
    startup.cb = sizeof(startup);
    PROCESS_INFORMATION process{};
    Require(CreateProcessW(self.c_str(), commandLine.data(), nullptr, nullptr, TRUE, 0, nullptr, nullptr, &startup, &process) != 0, "CreateProcessW failed");
    WaitForSingleObject(process.hProcess, INFINITE);
    DWORD code = 1;
    GetExitCodeProcess(process.hProcess, &code);
    CloseHandle(process.hThread);
    CloseHandle(process.hProcess);
    return static_cast<int>(code);
#else
    const pid_t child = fork();
    Require(child >= 0, "fork failed");
    if (child == 0) {
        execl(self.c_str(), self.c_str(), mode, static_cast<char*>(nullptr));
        std::_Exit(127);
    }
    int status = 0;
    Require(waitpid(child, &status, 0) == child, "waitpid failed");
    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
#endif
}

int RunUnexpectedReturns() {
    _ZSt13set_terminatePFvvE_nid_postfix([] { std::_Exit(77); });
    _ZSt14set_unexpectedPFvvE_nid_postfix([] {});
    _ZSt10unexpectedv_nid_postfix();
    return 1;
}

void CheckUnexpectedTerminatesOnReturn() {
    Require(RunChild(OwnExecutable(), "--unexpected-returns") == 77, "returning unexpected handler falls through to terminate");
}

}

int main(int argc, char** argv) {
    if (argc > 1 && std::strcmp(argv[1], "--unexpected-returns") == 0) return RunUnexpectedReturns();
    CheckUnexpectedHandlerLifecycle();
    CheckUnexpectedInvocation();
    CheckUnexpectedTerminatesOnReturn();
    std::puts("libc internal unexpected tests passed");
    return 0;
}
