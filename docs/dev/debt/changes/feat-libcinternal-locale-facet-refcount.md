# libSceLibcInternal

## remove _ZNSt6locale5facet7_DecrefEv

## remove _ZNSt6locale5facet7_IncrefEv

## add Functional

- [locale::facet::_Incref and _Decref](../../../core/libs/prx/libSceLibcInternal/InventoryExports.cpp) (libSceLibcInternal, NIDs `-EgSegeAKl4` and `AgxEl+HeWRQ`): the console's `libSceLibcInternal.sprx` was not read and no title was seen calling them. Following Dinkumware [crt.vc12 `xlocale`](https://github.com/ganboing/crt.vc12/blob/f1d47126bf13d1c9dccaa6c53b72217fd0474968/src/xlocale#L143-L157) and the facet layout of `GuestLocale::Facet`, `_Incref` atomically increments and `_Decref` atomically decrements the 32-bit count at offset 8 and returns the facet when the count reaches zero, otherwise null, as libc's `RetainFacet`/`ReleaseFacet` do; the upper 32 bits a constructor writes for a 64-bit `size_t` count are not touched, and null facets throw `std::invalid_argument`.
