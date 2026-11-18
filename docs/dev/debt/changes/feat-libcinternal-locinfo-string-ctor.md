# libSceLibcInternal

## remove _ZNSt8_LocinfoC1ERKSs

## remove _ZNSt8_LocinfoC2ERKSs

## add Functional

- [_ZNSt8_LocinfoC1ERKSs and _ZNSt8_LocinfoC2ERKSs](../../../core/libs/prx/libSceLibcInternal/LocaleObjects.cpp) (NIDs `2aSk2ruCP0E` and `ReK9U6EUWuU`) take the locale name from the guest `std::string` (`GuestLocale::String`, inline buffer while the capacity is at most 15) and run the `libc` export `_ZNSt8_LocinfoC1EPKc` with it, so only the `C` locale is accepted. `C2` does the same work as `C1` because `_Locinfo` has no virtual bases ([Itanium C++ ABI](https://itanium-cxx-abi.github.io/cxx-abi/abi.html#definitions)). Neither the [Dinkumware reference tree](https://github.com/dorpxtos/xtos/blob/8a9c9ff53a9cff0223fa4ef437236ad72f9482ff/Crt/stl/locale0.cpp#L208-L216) nor Microsoft STL has a `const string&` overload, so forwarding the characters to the `const char*` constructor is inferred from the mangled name; a null string or a name containing a null character throws `std::invalid_argument`. The console's `libSceLibcInternal.sprx` was not read and no title was seen calling them.
