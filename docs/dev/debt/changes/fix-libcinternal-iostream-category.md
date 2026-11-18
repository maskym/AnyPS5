# libSceLibcInternal

## remove `_ZNKSt24_Iostream_error_category7messageEi` (NID `n9-NJEULZ-0`)

## change [_ZdlPvmRKSt9nothrow_t, _ZdaPvmRKSt9nothrow_t and _ZSt17iostream_categoryv]

- [_ZdlPvmRKSt9nothrow_t, _ZdaPvmRKSt9nothrow_t, _ZSt17iostream_categoryv and _ZNKSt24_Iostream_error_category7messageEi](../../../core/libs/prx/libSceLibcInternal/CxxRuntimeForwards.cpp) (libSceLibcInternal, NIDs `7VPIYFpwU2A`, `7lCihI18N9I`, `V23qt24VPVs` and `n9-NJEULZ-0`) take the signatures their mangled names give: the sized `nothrow` scalar and array `operator delete`, which release the block on the application heap like the `libc` sized deletes and ignore the size, `std::iostream_category()`, which returns the same guest `_Iostream_error_category` object as the `libc` export, and that category's `message`, which forwards to `libc`'s. The console's `libSceLibcInternal.sprx` was not read and no title was seen calling them, so whether its deletes share `libc.prx`'s heap and whether its category object is distinct from `libc.prx`'s are unverified.

# libc

## add Functional

- [std::iostream_category](../../../core/libs/prx/libc/src/exception/StandardExceptions.cpp) (libc) returns a guest `_Iostream_error_category` object instead of the host one, whose vtable a title could not call. Its vtable has the slot order of the generic and system categories and its typeinfo derives from `_Generic_error_category`, as in [microsoft/STL system_error](https://github.com/microsoft/STL/blob/219514876ea86491de191ceaa88632616bbc0f19/stl/inc/system_error#L455-L472) and Dinkumware's [VC12 system_error](https://github.com/ganboing/crt.vc12/blob/f1d47126bf13d1c9dccaa6c53b72217fd0474968/src/system_error#L570-L590): `name` is "iostream", and `message` returns "iostream stream error" for `io_errc::stream` (1) and the generic message otherwise. The deleting destructor slot throws, as the future category's does. Whether the console's object, typeinfo and vtable match `libc.prx`'s is unverified, the typeinfo and vtable are not exported, and no title was seen calling them
