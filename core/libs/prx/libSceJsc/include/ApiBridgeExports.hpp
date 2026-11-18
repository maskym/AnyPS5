#pragma once

#include "SceTypes.hpp"
#include "prx/libSceJsc/include/Api.hpp"

namespace Jsc {

using WeakObjectMapDestroyedCallback = void(APS5_VABI*)(void* map, void* data);

struct ApiBridge {
    const void* (*makeString)(const void* context, const GuestJSString* string);
    bool (*hasPropertyForKey)(const void* context, const void* object, const void* key, const void** exception);
    void* (*weakObjectMapCreate)(const void* context, void* privateData, WeakObjectMapDestroyedCallback callback);
};

}

extern "C" {

const void* APS5_VABI JSValueMakeString(const void* context, const GuestJSString* string);
bool APS5_VABI JSObjectHasPropertyForKey(const void* context, const void* object, const void* key, const void** exception);
void* APS5_VABI JSWeakObjectMapCreate(const void* context, void* privateData, Jsc::WeakObjectMapDestroyedCallback callback);
bool APS5_VABI JSWeakObjectMapClear(const void* context, void* map, void* key, const void* object);
void RegisterApiBridge_nid_no_patch(const Jsc::ApiBridge* bridge);

}
