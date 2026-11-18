# libSceLibcInternal

## change `_ZNKSt13bad_exception8_DoraiseEv` (NID `kTlQY47fo88`), `_ZNKSt19istreambuf_iteratorIcSt11char_traitsIcEE5equalERKS2_`

- [Additional 3.20 imports](../../../core/libs/prx/libSceLibcInternal/InventoryExports2.cpp) (libSceLibcInternal) - `_ZNKSt13bad_exception8_DoraiseEv` (NID `kTlQY47fo88`); signatures and behavior are unverified and calls throw through `NotImplemented_nid_no_patch`.

## add Functional

- [istreambuf_iterator equal](../../../core/libs/prx/libSceLibcInternal/TimeGetFields.cpp) (libSceLibcInternal) - `_ZNKSt19istreambuf_iteratorIcSt11char_traitsIcEE5equalERKS2_` (NID `u6UfGT9+HEA`) and `_ZNKSt19istreambuf_iteratorIwSt11char_traitsIwEE5equalERKS2_` (NID `jZmLD-ASDto`) peek each iterator that has not fetched a character and return true when both or neither reached end of stream, as [xlocale](https://github.com/microsoft/STL/blob/770da5aa457af11f0a796600e1aa6f7f6fce14a6/stl/inc/xlocale) does. They reuse the iterator layout (buffer pointer, got byte, cached element at byte 1 or bytes 2-3) and the streambuf get pointers and `underflow` slot that time_get already assumes; none of these was compared with the module or a title's call. Closes when: compared with the module or a title's call
