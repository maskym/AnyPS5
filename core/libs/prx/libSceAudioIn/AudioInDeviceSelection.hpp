#pragma once

#include <cstdlib>
#include <string>
#include <string_view>

#ifdef _WIN32
#include <windows.h>
#endif

namespace audioin::detail {

enum class CaptureDeviceMatchKind {
    Default,
    Found,
    Missing,
    Ambiguous,
};

struct CaptureDeviceMatch {
    CaptureDeviceMatchKind kind;
    int index;
};

inline std::string configuredCaptureDevice() {
#ifdef _WIN32
    const DWORD size = GetEnvironmentVariableW(L"ANYPS5_AUDIO_IN_DEVICE", nullptr, 0);
    if (size <= 1) return {};
    std::wstring wide(size, L'\0');
    const DWORD length = GetEnvironmentVariableW(L"ANYPS5_AUDIO_IN_DEVICE", wide.data(), size);
    if (length == 0 || length >= size) return {};
    wide.resize(length);
    const int bytes = WideCharToMultiByte(CP_UTF8, 0, wide.data(), static_cast<int>(wide.size()), nullptr, 0, nullptr, nullptr);
    std::string value(static_cast<std::size_t>(bytes), '\0');
    WideCharToMultiByte(CP_UTF8, 0, wide.data(), static_cast<int>(wide.size()), value.data(), bytes, nullptr, nullptr);
    return value;
#else
    const char* value = std::getenv("ANYPS5_AUDIO_IN_DEVICE");
    return value ? std::string(value) : std::string();
#endif
}

template <typename TGetName>
CaptureDeviceMatch selectCaptureDevice(std::string_view requestedName, int deviceCount, TGetName&& getName) {
    if (requestedName.empty() || requestedName == "default") return {CaptureDeviceMatchKind::Default, -1};
    if (deviceCount < 0) return {CaptureDeviceMatchKind::Missing, -1};
    int match = -1;
    for (int index = 0; index < deviceCount; ++index) {
        const char* name = getName(index);
        if (!name || requestedName != name) continue;
        if (match >= 0) return {CaptureDeviceMatchKind::Ambiguous, -1};
        match = index;
    }
    if (match < 0) return {CaptureDeviceMatchKind::Missing, -1};
    return {CaptureDeviceMatchKind::Found, match};
}

}
