#include "prx/libc/include/GuestLocale.hpp"
#include "prx/libc/include/General.hpp"
#include "SceTypes.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace {

template<std::size_t Width>
int PutElements(GuestLocale::Streambuf* self, const char* source, int count, const char* caller) {
    if (self == nullptr || self->vtable == nullptr || (count > 0 && source == nullptr)) NotImplemented_nid_no_patch(caller);
    int written = 0;
    while (written < count) {
        char* next = self->putNext != nullptr ? *self->putNext : nullptr;
        std::int32_t avail = 0;
        if (next != nullptr && self->putEnd != nullptr) {
            const auto bytes = static_cast<std::intptr_t>(reinterpret_cast<std::uintptr_t>(*self->putEnd) - reinterpret_cast<std::uintptr_t>(next));
            avail = static_cast<std::int32_t>(static_cast<std::uint32_t>(bytes / static_cast<std::intptr_t>(Width)));
        }
        if (avail > 0) {
            const int n = std::min(static_cast<int>(avail), count - written);
            std::memcpy(next, source + static_cast<std::size_t>(written) * Width, static_cast<std::size_t>(n) * Width);
            *self->putNext = next + static_cast<std::size_t>(n) * Width;
            written += n;
            continue;
        }
        int element = 0;
        if constexpr (Width == 1) {
            element = static_cast<unsigned char>(source[written]);
        } else {
            std::uint16_t wide = 0;
            std::memcpy(&wide, source + static_cast<std::size_t>(written) * Width, sizeof(wide));
            element = wide;
        }
        if (self->vtable->overflow(self, element) == -1) break;
        ++written;
    }
    return written;
}

}

extern "C" {

int APS5_VABI sceLibcInternalStreambufXsputn_nid_no_patch(GuestLocale::Streambuf* self, const char* source, int count) {
    return PutElements<1>(self, source, count, __func__);
}
APS5_EXPORT("sXaxl1QGorg", sceLibcInternalStreambufXsputn_nid_no_patch);

int APS5_VABI sceLibcInternalWideStreambufXsputn_nid_no_patch(GuestLocale::Streambuf* self, const std::uint16_t* source, int count) {
    return PutElements<2>(self, reinterpret_cast<const char*>(source), count, __func__);
}
APS5_EXPORT("041c37QfoUc", sceLibcInternalWideStreambufXsputn_nid_no_patch);

}
