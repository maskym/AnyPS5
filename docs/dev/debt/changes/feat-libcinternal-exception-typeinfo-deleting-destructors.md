# libSceLibcInternal

## remove `lX9M5u0e48k`

## remove `N5nZ3wQw+Vc`

## add Functional

- [bad_exception and type_info deleting destructors](../../../core/libs/prx/libSceLibcInternal/InventoryExports6.cpp) (libSceLibcInternal, NIDs `lX9M5u0e48k`, `N5nZ3wQw+Vc`): the `D0` of `std::bad_exception` and `std::type_info` release the object through `_ZdlPv`, as the wide `money_put` `D0` does, because their `D1` and `D2` are a single `ret` in firmware 13.42 `libSceLibcInternal.sprx`. The console's module was not read for `D0` and no title was seen calling them. Closes when: compared with the module or a title's call
