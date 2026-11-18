# libc

## add Functional

- [ios_base::failure constructors](../../../core/libs/prx/libc/src/exception/StandardExceptions.cpp) (libc, NIDs `2PxU-rB3ixI`, `xFkYamgQChU`, `igWZiDXnBC0`) - the console module's bodies and the exporting module are unverified. `failure(const char*, const error_code&)` (C1 and C2) builds the 32-byte object as [Microsoft STL `xiosbase`](https://github.com/microsoft/STL/blob/e1ed302683e5e822f663fbb0f44cee7e9ddc6af0/stl/inc/xiosbase#L107-L108) does through `system_error(error_code, const char*)`, so the message is composed like `_ZNSt12system_errorC2ESt10error_codePKc`, and then takes the `ios_base::failure` vtable. The copy constructor shares the refcounted message and copies the 16-byte code. A null `error_code` reference or a null category throws `std::invalid_argument`, where Dinkumware dereferences it; a null message becomes empty. Closes when: compared with the console libc or libSceLibcInternal module.
