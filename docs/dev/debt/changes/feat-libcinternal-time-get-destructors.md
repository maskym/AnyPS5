# libSceLibcInternal

## remove mmq9OwwYx74

## remove Cp9ksNOeun8

## remove QTgRx1NTp6o

## add Functional

- [time_get destructors](../../../core/libs/prx/libSceLibcInternal/TimeGetFields.cpp) (libSceLibcInternal, NIDs `mmq9OwwYx74`, `Cp9ksNOeun8`, `QTgRx1NTp6o`) are unverified on a console: the `time_get<char>` and `time_get<wchar_t>` complete-object destructors run the base-object destructor and the `time_get<char>` deleting destructor then releases the object through `operator delete`, per the [Itanium C++ ABI](https://itanium-cxx-abi.github.io/cxx-abi/abi.html#mangling-special-ctor-dtor) for classes without virtual bases; like D2 they keep the facet's static tables unfreed.
