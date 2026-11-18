# libSceLibcInternal

## remove ww3UUl317Ng

## remove HpAlvhNKb2Y

## add Additional PS5 3.20 import declarations

- [_ZNKSt22_System_error_category7messageEi and _ZNKSt22_System_error_category23default_error_conditionEi](../../../core/libs/prx/libSceLibcInternal/ConstantMembers.cpp) (libSceLibcInternal, NIDs `HpAlvhNKb2Y` and `ww3UUl317Ng`) were not compared with the console's `libSceLibcInternal.sprx` and no title was seen calling them. They run the `message` and `default_error_condition` entries of the `libc` system category vtable in [StandardExceptions.cpp](../../../core/libs/prx/libc/src/exception/StandardExceptions.cpp): the generic message or `unknown error`, and the generic category for values `_Syserror_map` names, the system category otherwise. Closes when: compared with the module
