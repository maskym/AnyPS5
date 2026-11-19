#include "prx/libSceAudioOut/include/OutputDevice.hpp"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

namespace {

void require(bool condition, const char* reason) {
    if (!condition) {
        std::fprintf(stderr, "%s\n", reason);
        std::exit(1);
    }
}

void clearDevice() {
#ifdef _WIN32
    _wputenv_s(L"ANYPS5_AUDIO_DEVICE", L"");
#else
    unsetenv("ANYPS5_AUDIO_DEVICE");
#endif
}

void setAsciiDevice() {
#ifdef _WIN32
    _wputenv_s(L"ANYPS5_AUDIO_DEVICE", L"USB Headset");
#else
    setenv("ANYPS5_AUDIO_DEVICE", "USB Headset", 1);
#endif
}

}

int main() {
    const std::vector<std::string> devices{"Built-in Audio", "USB Headset"};
    require(FindOutputDevice(devices, "USB Headset") == std::optional<std::size_t>{1}, "a listed device is found by its exact name");
    require(FindOutputDevice(devices, "usb headset") == std::nullopt, "device names are matched exactly, not by case");
    require(FindOutputDevice(devices, "Missing") == std::nullopt, "an unlisted device is not found");
    require(FindOutputDevice({}, "Built-in Audio") == std::nullopt, "an empty device list finds nothing");
    clearDevice();
    require(ConfiguredOutputDevice().empty(), "an unset ANYPS5_AUDIO_DEVICE configures no device");
    setAsciiDevice();
    require(ConfiguredOutputDevice() == "USB Headset", "ANYPS5_AUDIO_DEVICE is read back unchanged");
#ifdef _WIN32
    _wputenv_s(L"ANYPS5_AUDIO_DEVICE", L"\u00C9couteurs (\u0420\u0435\u0430\u043B\u0442\u0435\u043A)");
    const std::string utf8 = "\xC3\x89" "couteurs (\xD0\xA0\xD0\xB5\xD0\xB0\xD0\xBB\xD1\x82\xD0\xB5\xD0\xBA)";
    require(ConfiguredOutputDevice() == utf8, "a non-ASCII ANYPS5_AUDIO_DEVICE is read as UTF-8");
    require(FindOutputDevice({utf8, "USB Headset"}, ConfiguredOutputDevice()) == std::optional<std::size_t>{0}, "a non-ASCII ANYPS5_AUDIO_DEVICE selects the SDL device with the same UTF-8 name");
#endif
    std::puts("audio output device selection passed");
    return 0;
}
