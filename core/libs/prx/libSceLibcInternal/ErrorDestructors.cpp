#include "prx/libSceLibcInternal/include/Api.hpp"
#include "prx/libc/include/Api.hpp"
#include "prx/libc/include/General.hpp"
#include "SceTypes.hpp"

extern "C" {

void APS5_VABI sceLibcInternalOverflowErrorDelete_nid_no_patch(LibcException::ExceptionObject* self) {
    _ZNSt13runtime_errorD0Ev_nid_postfix(self);
}
APS5_EXPORT("y1dYQsc67ys", sceLibcInternalOverflowErrorDelete_nid_no_patch);

void APS5_VABI sceLibcInternalUnderflowErrorDelete_nid_no_patch(LibcException::ExceptionObject* self) {
    _ZNSt13runtime_errorD0Ev_nid_postfix(self);
}
APS5_EXPORT("iHILAmwYRGY", sceLibcInternalUnderflowErrorDelete_nid_no_patch);

void APS5_VABI sceLibcInternalFutureCategoryDestroy_nid_no_patch(LibcException::CategoryObject*) {}
APS5_EXPORT("xsXQD5ybREw", sceLibcInternalFutureCategoryDestroy_nid_no_patch);

void APS5_VABI sceLibcInternalPlacementDelete_nid_no_patch(void*, void*) {}
APS5_EXPORT("1vo6qqqa9F4", sceLibcInternalPlacementDelete_nid_no_patch);

void APS5_VABI sceLibcInternalPlacementArrayDelete_nid_no_patch(void*, void*) {}
APS5_EXPORT("Suc8W0QPxjw", sceLibcInternalPlacementArrayDelete_nid_no_patch);

}
