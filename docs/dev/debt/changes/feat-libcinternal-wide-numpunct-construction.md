# libSceLibcInternal

## remove `_ZNSt8numpunctIwEC1EPKcmb`

## remove `_ZNSt8numpunctIwEC1ERKSt8_Locinfomb`

## remove `_ZNSt8numpunctIwEC1Em`

## remove `_ZNSt8numpunctIwEC2EPKcmb`

## remove `_ZNSt8numpunctIwEC2ERKSt8_Locinfomb`

## remove `_ZNSt8numpunctIwEC2Em`

## remove `_ZNSt8numpunctIwED0Ev`

## remove `_ZNSt8numpunctIwED1Ev`

## remove `_ZNSt8numpunctIwED2Ev`

## remove [_ZNSt8numpunctIwE5_InitERKSt8_Locinfob]

## add Functional

- [numpunct<wchar_t> constructors, _Init and destructors](../../../core/libs/prx/libSceLibcInternal/Numpunct.cpp) (libSceLibcInternal, NIDs `XOgjMgZ3fjo`, `ElKI+ReiehU`, `s1EM2NdPf0Y`, `MQJQCxbLfM0`, `H+T2VJ91dds`, `m4kEqv7eGVY`, `uiRALKOdAZk`, `VHBnRBxBg5E`, `lzK3uL1rWJY`, `XDm4jTtoEbo`) follow [xlocnum](https://github.com/microsoft/STL/blob/219514876ea86491de191ceaa88632616bbc0f19/stl/inc/xlocnum#L137-L229).
  - The facet gets libc's wide numpunct vtable and a 64-bit reference count.
  - `_Init` takes grouping and separators from libc's `localeconv` (`'.'`/`','` and empty grouping when `_Isdef`), widening the narrow bytes because the guest `lconv` has no `_W_` fields. False and true names are static.
  - `numpunct(size_t)` replaces a zero separator with `','`.
  - Names other than C throw, because `_Locinfo` accepts only C.
  - The strings are never freed, so the destructors only check `self`. The deleting destructor frees through `operator delete`.
  - The console's `libSceLibcInternal.sprx` was not read.
