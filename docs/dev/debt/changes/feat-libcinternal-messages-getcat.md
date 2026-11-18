# libSceLibcInternal

## remove `_ZNSt8messagesIcE7_GetcatEPPKNSt6locale5facetEPKS1_`

## remove `_ZNSt8messagesIwE7_GetcatEPPKNSt6locale5facetEPKS1_`

## add Functional

- [messages<char>::_Getcat and messages<wchar_t>::_Getcat](../../../core/libs/prx/libSceLibcInternal/Messages.cpp) (libSceLibcInternal, NIDs `96Ev+CE1luE`, `XghI4vmw8mU`) follow the Microsoft STL [`xlocmes`](https://github.com/microsoft/STL/blob/219514876ea86491de191ceaa88632616bbc0f19/stl/inc/xlocmes#L63-L70): when `*_Ppf` is null they allocate a 16-byte facet, build it with no references from `_Locinfo(_Ploc->c_str())`, freeing it if that throws, and they return `_X_MESSAGES`, 6 in [`xlocinfo.h`](https://github.com/microsoft/STL/blob/219514876ea86491de191ceaa88632616bbc0f19/stl/inc/xlocinfo.h#L47). Names `_Locinfo(const char*)` refuses throw, including the `""` that [`locale::c_str`](https://github.com/microsoft/STL/blob/219514876ea86491de191ceaa88632616bbc0f19/stl/inc/xlocale#L370-L372) gives a locale without an implementation; a null `_Ploc`, which the source dereferences, throws. The console's `libSceLibcInternal.sprx` was not read and no title was seen calling them.
