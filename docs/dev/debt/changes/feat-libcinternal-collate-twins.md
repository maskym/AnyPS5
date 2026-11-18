# libSceLibcInternal

## remove `L71JAnoQees`

## remove `tkqNipin1EI`

## remove `Lt4407UMs2o`

## remove `ig4VDIRc21Q`

## add Functional

- [collate<char> C2 constructors and collate deleting destructors](../../../core/libs/prx/libSceLibcInternal/Collate.cpp) (libSceLibcInternal, NIDs `tkqNipin1EI`, `L71JAnoQees`, `Lt4407UMs2o` and `ig4VDIRc21Q`): the console's `libSceLibcInternal.sprx` was not read and no title was seen calling these. Following [locale](https://github.com/microsoft/STL/blob/165a67afdde92bf3e5248be35b6f9240b3d1efb5/stl/inc/locale#L93-L124), `C2(size_t)` and `C2(const _Locinfo&, size_t)` run the same bodies as their `C1` twins, and `collate<char>`/`collate<wchar_t>` `D0` forward to the `deleteObject` slot of libc's collate vtables, which rejects a null object and releases it with `operator delete`; non-C locales remain unsupported.
