#include "prx/libSceLibcInternal/include/Api.hpp"
#include "prx/libc/include/Api.hpp"
#include "prx/libc/include/General.hpp"
#include "SceTypes.hpp"

extern "C" {

const char* APS5_VABI sceLibcInternalSystemErrorCategoryName_nid_no_patch(const void*) noexcept {
    return "system";
}
APS5_EXPORT("dXy+lFOiaQA", sceLibcInternalSystemErrorCategoryName_nid_no_patch);

GuestLocale::String* APS5_VABI sceLibcInternalSystemErrorCategoryMessage_nid_no_patch(GuestLocale::String* result, const LibcException::CategoryObject* self, int value) {
    using Message = GuestLocale::String* (APS5_VABI*)(GuestLocale::String*, const LibcException::CategoryObject*, int);
    return reinterpret_cast<Message>(const_cast<void*>(static_cast<const void* const*>(_ZSt15system_categoryv_nid_postfix()->vtable)[3]))(result, self, value);
}
APS5_EXPORT("HpAlvhNKb2Y", sceLibcInternalSystemErrorCategoryMessage_nid_no_patch);

LibcException::ErrorCode APS5_VABI sceLibcInternalSystemErrorCategoryCondition_nid_no_patch(const LibcException::CategoryObject* self, int value) {
    using Condition = LibcException::ErrorCode (APS5_VABI*)(const LibcException::CategoryObject*, int);
    return reinterpret_cast<Condition>(const_cast<void*>(static_cast<const void* const*>(_ZSt15system_categoryv_nid_postfix()->vtable)[4]))(self, value);
}
APS5_EXPORT("ww3UUl317Ng", sceLibcInternalSystemErrorCategoryCondition_nid_no_patch);

const char* APS5_VABI sceLibcInternalGenericErrorCategoryName_nid_no_patch(const void*) noexcept {
    return "generic";
}
APS5_EXPORT("xvVR0CBPFAM", sceLibcInternalGenericErrorCategoryName_nid_no_patch);

const char* APS5_VABI sceLibcInternalIostreamErrorCategoryName_nid_no_patch(const void*) noexcept {
    return "iostream";
}
APS5_EXPORT("sb2vivqtLS0", sceLibcInternalIostreamErrorCategoryName_nid_no_patch);

const char* APS5_VABI sceLibcInternalFutureErrorCategoryName_nid_no_patch(const void*) noexcept {
    return "future";
}
APS5_EXPORT("Q0VsWTapQ4M", sceLibcInternalFutureErrorCategoryName_nid_no_patch);

int APS5_VABI sceLibcInternalCodecvtBaseMaxLength_nid_no_patch(const void*) noexcept {
    return 1;
}
APS5_EXPORT("FOsY+JAyXow", sceLibcInternalCodecvtBaseMaxLength_nid_no_patch);

int APS5_VABI sceLibcInternalCodecvtBaseEncoding_nid_no_patch(const void*) noexcept {
    return 1;
}
APS5_EXPORT("MwAySqTo+-M", sceLibcInternalCodecvtBaseEncoding_nid_no_patch);

const char* APS5_VABI sceLibcInternalFutureErrorMap_nid_no_patch(int code) noexcept {
    switch (code) {
    case 1: return "broken promise";
    case 2: return "future already retrieved";
    case 3: return "promise already satisfied";
    case 4: return "no state";
    default: return nullptr;
    }
}
APS5_EXPORT("0r8rbw2SFqk", sceLibcInternalFutureErrorMap_nid_no_patch);

const char* APS5_VABI _ZSt22_Get_future_error_whati_nid_postfix(int code) noexcept {
    return sceLibcInternalFutureErrorMap_nid_no_patch(code);
}

}
