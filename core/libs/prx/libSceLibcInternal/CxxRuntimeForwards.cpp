#include "prx/libSceLibcInternal/include/Api.hpp"
#include "prx/libc/include/Api.hpp"
#include "prx/libc/include/General.hpp"
#include "SceTypes.hpp"
#include <cstddef>

extern "C" {

void APS5_VABI sceLibcInternalSizedNothrowDelete_nid_no_patch(void* pointer, std::size_t size, const void*) noexcept {
    _ZdlPvm_nid_postfix(pointer, size);
}
APS5_EXPORT("7VPIYFpwU2A", sceLibcInternalSizedNothrowDelete_nid_no_patch);

void APS5_VABI sceLibcInternalSizedNothrowArrayDelete_nid_no_patch(void* pointer, std::size_t size, const void*) noexcept {
    _ZdaPvm_nid_postfix(pointer, size);
}
APS5_EXPORT("7lCihI18N9I", sceLibcInternalSizedNothrowArrayDelete_nid_no_patch);

const LibcException::CategoryObject* APS5_VABI sceLibcInternalIostreamCategory_nid_no_patch() {
    return IostreamCategory_nid_no_patch();
}
APS5_EXPORT("V23qt24VPVs", sceLibcInternalIostreamCategory_nid_no_patch);

GuestLocale::String* APS5_VABI sceLibcInternalIostreamCategoryMessage_nid_no_patch(GuestLocale::String* result, const LibcException::CategoryObject* self, int value) {
    return IostreamCategoryMessage_nid_no_patch(result, self, value);
}
APS5_EXPORT("n9-NJEULZ-0", sceLibcInternalIostreamCategoryMessage_nid_no_patch);

}
