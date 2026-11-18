# libSceLibcInternal

## remove | `_new_setup` | `KNNNbyRieqQ` |

## add Functional

- [_new_setup](../../../core/libs/prx/libSceLibcInternal/InventoryExports.cpp) (libSceLibcInternal, NID `KNNNbyRieqQ`) forwards to libc's `_new_setup`, so it records the CRT function table and answers with the accept/reject rule of the `libc.prx` export at `0x186a0` (see [libc](libc.md)): 1 for a size below `0x50` or a slot group that is neither all present nor all absent, 0 otherwise. The console's `libSceLibcInternal.sprx` copy was not disassembled, so whether it applies the same rule is unverified, and here both modules record into one slot table, which is also unverified
