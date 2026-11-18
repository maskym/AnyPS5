# libSceLibcInternal

## remove _ZNKSt7num_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEE5_FputES3_RSt8ios_basecPKcm](

## remove _ZNKSt7num_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEE5_IputES3_RSt8ios_basecPcm](

## add Functional
- [num_put<char>::_Iput and _Fput](../../../core/libs/prx/libSceLibcInternal/FacetPut.cpp) (libSceLibcInternal, NIDs `jykT-VWQVBY` and `ll99KkqO6ig`) take the prototype from their mangled names and pad, group and write the already formatted text through the same code as `libc`'s `num_put<char>::do_put`, whose integer and floating tails follow the SDK 7.00 `libc.prx` (see the `num_put<char>::do_put` entry of `libc.md`); the facet argument is unused. The console's `libSceLibcInternal.sprx` bodies were not read, so their equality with that tail is assumed, and `_Iput` widens a copy of the digits instead of rewriting the caller's buffer in place as Dinkumware does. No title was seen calling them.
