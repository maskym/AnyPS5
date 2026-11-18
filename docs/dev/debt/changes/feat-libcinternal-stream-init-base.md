# libSceLibcInternal

## remove | `_ZNSt6_WinitC2Ev` | `57mMrw0l-40` |

## remove | `_ZNSt8ios_base4InitD2Ev` | `bxLH5WHgMBY` |

## remove [_ZNSt8ios_base4InitC2Ev](../../../core/libs/prx/libSceLibcInternal/InventoryExports6.cpp)

## add Functional

- [_ZNSt8ios_base4InitC2Ev, _ZNSt8ios_base4InitD2Ev and _ZNSt6_WinitC2Ev](../../../core/libs/prx/libSceLibcInternal/InventoryExports.cpp) (libSceLibcInternal, NIDs `bTQcNwRc8hE`, `bxLH5WHgMBY`, `57mMrw0l-40`) - not checked against `libSceLibcInternal.sprx` or on a console. The base-object forms share `narrowStreamInitCount` or `wideStreamInitCount` and `CountStandardStreamInit` or `ReleaseStandardStreamInit` with the complete-object `C1` and `D1` forms, so the last destruction through either form flushes libc's `_Stdout` and `_Stderr`; under the [Itanium C++ ABI](https://itanium-cxx-abi.github.io/cxx-abi/abi.html#definitions) the base-object constructor and destructor of a class without virtual bases do the same work as the complete-object ones.
