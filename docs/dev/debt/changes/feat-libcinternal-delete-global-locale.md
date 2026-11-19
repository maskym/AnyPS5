# libSceLibcInternal

## change `_Deletegloballocale`, `_Dtento`, `_Erfc`, `_FAtan`

- [Additional imported names](../../../core/libs/prx/libSceLibcInternal/Export.cpp) (libSceLibcInternal) - `_Dtento`, `_Erfc`, `_FAtan`

## change `_Deletegloballocale`, `_Erfc`, `_FAtan`

- [Additional imported names](../../../core/libs/prx/libSceLibcInternal/Export.cpp) (libSceLibcInternal) - `_Erfc`, `_FAtan`

## add Functional

- [_Deletegloballocale](../../../core/libs/prx/libSceLibcInternal/Export.cpp) (libSceLibcInternal) - not compared with the console module. Follows [Microsoft STL](https://github.com/microsoft/STL/blob/b53b100c3c08f3e2a4453bbe75e3156d4127ffc1/stl/src/locale0.cpp#L81-L86): reads the `_Locimp*` stored at the pointer, does nothing when it is null, and otherwise releases one reference and deletes the locale when that was the last one, through the locale's release and delete vtable slots. The stored pointer is left unchanged. A null pointer or an empty vtable slot throws. Closes when: compared with the PS5 module.
