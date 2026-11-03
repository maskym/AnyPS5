#include "prx/libSceLibcInternal/include/Api.hpp"
#include "prx/libc/include/Api.hpp"
#include "SceTypes.hpp"

extern "C" {

void APS5_VABI sceLibcInternalFutureCategoryDestroy_nid_no_patch(LibcException::CategoryObject* self) {
    _ZNSt14error_categoryD2Ev_nid_postfix(self);
}
APS5_EXPORT("xsXQD5ybREw", sceLibcInternalFutureCategoryDestroy_nid_no_patch);

}
