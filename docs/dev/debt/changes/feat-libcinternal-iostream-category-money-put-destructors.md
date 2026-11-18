# libSceLibcInternal

## remove `27Z-Cx1jbkU`

## remove `s2zG12AYKTg`

## add Functional

- [_ZNSt24_Iostream_error_categoryD1Ev](../../../core/libs/prx/libSceLibcInternal/ExceptionDestructors.cpp) (libSceLibcInternal, NID `27Z-Cx1jbkU`) follows `_ZNSt22_Future_error_categoryD1Ev`: `_Iostream_error_category` adds no member to `error_category`, so `D1` runs the `libc` export `_ZNSt14error_categoryD2Ev`, which frees nothing. The console's `libSceLibcInternal.sprx` was not read and no title was seen calling it. Closes when: compared with the module or a title's call
- [_ZNSt9money_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEED0Ev](../../../core/libs/prx/libSceLibcInternal/InventoryExports6.cpp) (libSceLibcInternal, NID `s2zG12AYKTg`) releases the object through `_ZdlPv`, as the wide `money_put` `D0` does, because its `D1` and `D2` do nothing. The console's `libSceLibcInternal.sprx` was not read and no title was seen calling it. Closes when: compared with the module or a title's call
