# libSceLibcInternal

## remove `-1G1iE3KyGI`

## remove `aHDdLa7jA1U`

## remove `aOK5ucXO-5g`

## remove `jyXTVnmlJD4`

## remove `b9QSruV4nnc`

## remove `RB3ratfpZDc`

## add Functional

- [num_get, num_put and money_get deleting destructors](../../../core/libs/prx/libSceLibcInternal/InventoryExports6.cpp) (libSceLibcInternal, NIDs `-1G1iE3KyGI`, `aHDdLa7jA1U`, `aOK5ucXO-5g`, `jyXTVnmlJD4`, `b9QSruV4nnc`, `RB3ratfpZDc`): the `D0` of `num_get<char>`, `num_get<wchar_t>`, `num_put<char>`, `num_put<wchar_t>`, `money_get<char>` and `money_get<wchar_t>` release the object through `_ZdlPv`, as the wide `money_put` `D0` does, because their `D1` and `D2` do nothing and the `libc` `num_put<char>` facet vtable already deletes through `_ZdlPv`. The console's `libSceLibcInternal.sprx` was not read for `D0` and no title was seen calling them. Closes when: compared with the module or a title's call
