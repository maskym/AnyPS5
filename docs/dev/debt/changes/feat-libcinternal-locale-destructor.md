# libSceLibcInternal

## add Functional

- [std::locale destructor](../../../core/libs/prx/libSceLibcInternal/InventoryExports.cpp) (libSceLibcInternal, NID `pMWnITHysPc`, exported under its inventory alias) follows Dinkumware's `~locale` ([microsoft/STL `xlocale`](https://github.com/microsoft/STL/blob/219514876ea86491de191ceaa88632616bbc0f19/stl/inc/xlocale#L351-L355)). It calls the `_Locimp`'s `_Decref` slot (`FacetVtable::release`) and, when that returns an object, its deleting destructor (`deleteObject`); a null `_Ptr` does nothing. The classic locale and copies made by `_Locimp(const _Locimp&)` return null from `_Decref` here, so they are never freed. A null `this` throws `std::invalid_argument` where the console faults. The console's `libSceLibcInternal.sprx` body was not read and no title was seen calling it.

## remove `_ZNSt6localeD1Ev`
