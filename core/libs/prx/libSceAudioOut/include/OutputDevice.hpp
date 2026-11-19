#ifndef CORE_LIBS_PRX_LIBSCEAUDIOOUT_INCLUDE_OUTPUTDEVICE_HPP
#define CORE_LIBS_PRX_LIBSCEAUDIOOUT_INCLUDE_OUTPUTDEVICE_HPP

#include <cstddef>
#include <cstdlib>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

inline std::optional<std::size_t> FindOutputDevice(const std::vector<std::string>& names, std::string_view wanted) {
    for (std::size_t index = 0; index < names.size(); ++index) {
        if (names[index] == wanted) return index;
    }
    return std::nullopt;
}

inline std::string ConfiguredOutputDevice() {
#ifdef _WIN32
    const DWORD size = GetEnvironmentVariableW(L"ANYPS5_AUDIO_DEVICE", nullptr, 0);
    if (size <= 1) return {};
    std::wstring wide(size, L'\0');
    const DWORD length = GetEnvironmentVariableW(L"ANYPS5_AUDIO_DEVICE", wide.data(), size);
    if (length == 0 || length >= size) return {};
    wide.resize(length);
    const int bytes = WideCharToMultiByte(CP_UTF8, 0, wide.data(), static_cast<int>(wide.size()), nullptr, 0, nullptr, nullptr);
    std::string value(static_cast<std::size_t>(bytes), '\0');
    WideCharToMultiByte(CP_UTF8, 0, wide.data(), static_cast<int>(wide.size()), value.data(), bytes, nullptr, nullptr);
    return value;
#else
    const char* value = std::getenv("ANYPS5_AUDIO_DEVICE");
    return value ? std::string(value) : std::string();
#endif
}

#endif
