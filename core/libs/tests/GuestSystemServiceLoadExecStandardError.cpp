#include "prx/libkernel/include/Api.hpp"
#include "prx/libSceSystemService/include/SystemService.hpp"
#include "prx/libc/include/general/VabiMacros.hpp"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <string_view>
#ifndef _WIN32
#include <sys/wait.h>
#include <unistd.h>
#else
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif

namespace {

constexpr const char* Marker = "APS5_LOAD_EXEC_STANDARD_ERROR";
constexpr const char* AfterLoadExec = "--after-load-exec";

void Require(bool value) {
    if (!value) std::abort();
}

#ifndef _WIN32
std::string Drain(int descriptor) {
    std::string text;
    char buffer[4096];
    for (ssize_t count; (count = ::read(descriptor, buffer, sizeof(buffer))) > 0;) text.append(buffer, static_cast<std::size_t>(count));
    ::close(descriptor);
    return text;
}

bool HostOutputIs(const std::string& text, const std::string& line) {
    constexpr std::string_view WriteWatchUnavailable = "[memory] write watch unavailable: ";
    std::size_t begin = 0;
    if (text.compare(0, WriteWatchUnavailable.size(), WriteWatchUnavailable) == 0) {
        const std::size_t end = text.find('\n');
        if (end == std::string::npos) return false;
        begin = end + 1;
    }
    return text.compare(begin, std::string::npos, line) == 0;
}

void Run(const char* mode, const char* const* arguments) {
    int host[2] = {-1, -1};
    int guest[2] = {-1, -1};
    Require(::pipe(host) == 0);
    Require(::pipe(guest) == 0);
    const pid_t child = ::fork();
    Require(child >= 0);
    if (child == 0) {
        ::close(host[0]);
        ::close(guest[0]);
        if (::dup2(host[1], 2) != 2) ::_exit(70);
        ::close(host[1]);
        if (dup2_nid_postfix(guest[1], 2) != 2) ::_exit(71);
        ::close(guest[1]);
        if (::setenv(Marker, mode, 1) != 0) ::_exit(72);
        sceSystemServiceLoadExec("/app0/eboot.bin", arguments);
        ::_exit(73);
    }
    ::close(host[1]);
    ::close(guest[1]);
    const std::string hostText = Drain(host[0]);
    const std::string guestText = Drain(guest[0]);
    int status = 0;
    Require(::waitpid(child, &status, 0) == child);
    Require(WIFEXITED(status) && WEXITSTATUS(status) == 0);
    Require(HostOutputIs(hostText, std::string("started ") + mode + "\n"));
    Require(guestText.empty());
}
#else
constexpr const char* GuestHandle = "APS5_LOAD_EXEC_GUEST_HANDLE";
constexpr const char* Child = "--child";

std::string Drain(HANDLE handle) {
    std::string text;
    char buffer[4096];
    for (DWORD count = 0; ReadFile(handle, buffer, sizeof(buffer), &count, nullptr) && count > 0;) text.append(buffer, count);
    CloseHandle(handle);
    return text;
}

bool HostOutputIs(const std::string& text, const std::string& line) {
    std::size_t begin = 0;
    while (begin < text.size() && text[begin] == '[') {
        const std::size_t end = text.find('\n', begin);
        if (end == std::string::npos) return false;
        begin = end + 1;
    }
    return text.compare(begin, std::string::npos, line) == 0;
}

void Run(const char* mode) {
    SECURITY_ATTRIBUTES security{sizeof(security), nullptr, TRUE};
    HANDLE hostRead = nullptr, hostWrite = nullptr, guestRead = nullptr, guestWrite = nullptr;
    Require(CreatePipe(&hostRead, &hostWrite, &security, 0) != 0);
    Require(CreatePipe(&guestRead, &guestWrite, &security, 0) != 0);
    Require(SetHandleInformation(hostRead, HANDLE_FLAG_INHERIT, 0) != 0);
    Require(SetHandleInformation(guestRead, HANDLE_FLAG_INHERIT, 0) != 0);
    Require(SetEnvironmentVariableA(Marker, mode) != 0);
    Require(SetEnvironmentVariableA(GuestHandle, std::to_string(reinterpret_cast<std::uintptr_t>(guestWrite)).c_str()) != 0);
    std::string self(MAX_PATH, '\0');
    for (;;) {
        const DWORD length = GetModuleFileNameA(nullptr, self.data(), static_cast<DWORD>(self.size()));
        Require(length != 0);
        if (length < self.size()) {
            self.resize(length);
            break;
        }
        self.resize(self.size() * 2);
    }
    std::string commandLine = "\"" + self + "\" " + Child;
    STARTUPINFOA startup{};
    startup.cb = sizeof(startup);
    startup.dwFlags = STARTF_USESTDHANDLES;
    startup.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    startup.hStdOutput = hostWrite;
    startup.hStdError = hostWrite;
    PROCESS_INFORMATION process{};
    const BOOL created = CreateProcessA(self.c_str(), commandLine.data(), nullptr, nullptr, TRUE, 0, nullptr, nullptr, &startup, &process);
    Require(SetEnvironmentVariableA(Marker, nullptr) != 0);
    Require(SetEnvironmentVariableA(GuestHandle, nullptr) != 0);
    CloseHandle(hostWrite);
    CloseHandle(guestWrite);
    Require(created != 0);
    CloseHandle(process.hThread);
    const std::string hostText = Drain(hostRead);
    const std::string guestText = Drain(guestRead);
    Require(WaitForSingleObject(process.hProcess, INFINITE) == WAIT_OBJECT_0);
    DWORD code = 1;
    Require(GetExitCodeProcess(process.hProcess, &code) != 0);
    CloseHandle(process.hProcess);
    if (code != 0 || !HostOutputIs(hostText, std::string("started ") + mode + "\r\n") || !guestText.empty()) {
        std::fprintf(stderr, "mode %s: exit %lu, host \"%s\", guest \"%s\"\n", mode, static_cast<unsigned long>(code), hostText.c_str(), guestText.c_str());
        std::abort();
    }
}

int RunChild(const char* mode) {
    const char* handle = std::getenv(GuestHandle);
    Require(handle != nullptr);
    const int guest = _open_osfhandle(static_cast<intptr_t>(std::strtoull(handle, nullptr, 10)), _O_WRONLY | _O_BINARY);
    Require(guest >= 0);
    Require(dup2_nid_postfix(guest, 2) == 2);
    Require(_close(guest) == 0);
    const char* const arguments[] = {AfterLoadExec, nullptr};
    sceSystemServiceLoadExec("/app0/eboot.bin", std::strcmp(mode, "list") == 0 ? arguments : nullptr);
    return 73;
}
#endif

}

int main(int argc, char** argv) {
    if (const char* mode = std::getenv(Marker); mode != nullptr) {
#ifdef _WIN32
        if (argc == 2 && std::strcmp(argv[1], Child) == 0) return RunChild(mode);
        if (std::strcmp(mode, "list") == 0) Require(argc == 2 && std::strcmp(argv[1], AfterLoadExec) == 0);
        else Require(std::strcmp(mode, "none") == 0 && argc == 1);
#else
        if (std::strcmp(mode, "list") == 0) Require(argc == 1 && std::strcmp(argv[0], AfterLoadExec) == 0);
        else Require(std::strcmp(mode, "none") == 0 && argc == 1);
#endif
        std::fprintf(stderr, "started %s\n", mode);
        return 0;
    }
#ifdef _WIN32
    Run("list");
    Run("none");
#else
    const char* const arguments[] = {AfterLoadExec, nullptr};
    Run("list", arguments);
    Run("none", nullptr);
#endif
    std::puts("LoadExec standard error tests passed");
    return 0;
}
