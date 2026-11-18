# libSceLibcInternal

## remove _ZNSt7num_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEEC1ERKSt8_Locinfom

## remove _ZNSt7num_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEEC1Em

## remove _ZNSt7num_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEEC2ERKSt8_Locinfom

## remove _ZNSt7num_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEED0Ev

## remove _ZNSt7num_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEEC2Em

## add Functional

- [num_put<char, ostreambuf_iterator<char>> constructors and deleting destructor](../../../core/libs/prx/libSceLibcInternal/FacetPut.cpp) (libSceLibcInternal, NIDs `LAEVU8cBSh4`, `1gYJIrfHxkQ`, `Hg1im-rUeHc`, `Mniutm2JL2M`, `aOK5ucXO-5g`) are unverified on a console: the constructors validate non-null `self` (and non-null `_Locinfo` for the `_Locinfo` overloads), write only the `0x10`-byte `GuestLocale::Facet` header (`&_ZTVSt7num_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEE_nid_postfix[2]` and 64-bit `refs`), and `D0` frees `self` through `_ZdlPv_nid_postfix`, because `_Init` is a single `ret` in firmware 13.42 and Microsoft STL [xlocnum](https://github.com/microsoft/STL/blob/219514876ea86491de191ceaa88632616bbc0f19/stl/inc/xlocnum#L1170-L1184) `num_put` has no other members.
