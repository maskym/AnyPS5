# libSceLibcInternal

## remove [_ZNSt7collateIwEC1Em]

## remove `_ZNSt7collateIwEC2Em`

## remove `_ZNSt7collateIwEC1EPKcm`

## remove `_ZNSt7collateIwEC2EPKcm`

## remove `_ZNSt7collateIwEC1ERKSt8_Locinfom`

## remove `_ZNSt7collateIwEC2ERKSt8_Locinfom`

## add Functional

- [collate<wchar_t> constructors](../../../core/libs/prx/libSceLibcInternal/WideCollate.cpp) (libSceLibcInternal, NIDs `d-MOtyu8GAk`, `HQbgeUdQyyw`, `fjHAU8OSaW8`, `PSAw7g1DD24`, `wggIIjWSt-E` and `2PoQu-K2qXk`): the console's `libSceLibcInternal.sprx` was not read and no title was seen calling these constructors. Following [locale](https://github.com/microsoft/STL/blob/165a67afdde92bf3e5248be35b6f9240b3d1efb5/stl/inc/locale#L93-L124), the `size_t` and `const char*` constructors validate the locale via libc's `_Locinfo`, which accepts only `"C"`, the `_Locinfo` constructors borrow it, null `self`/`_Locinfo` throw `std::invalid_argument`, and all six install libc's wide collate vtable (the one `collate<wchar_t>::_Getcat` uses), write 64-bit `references` and zero offsets `0x10` and `0x18`; non-C locales and `_Init` remain unsupported.
