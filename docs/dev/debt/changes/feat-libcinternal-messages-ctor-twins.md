# libSceLibcInternal

## remove `_ZNSt8messagesIcEC1Em`

## remove `_ZNSt8messagesIcEC2EPKcm`

## remove `_ZNSt8messagesIwEC1Em`

## remove `_ZNSt8messagesIwEC2EPKcm`

## remove `_ZNSt8messagesIwEC2ERKSt8_Locinfom`

## add Functional

- [messages<char> and messages<wchar_t> complete and base constructors](../../../core/libs/prx/libSceLibcInternal/Messages.cpp) (libSceLibcInternal, NIDs `6gCBQs1mIi4`, `qF3mHeMAHVk`, `n4+3hznhkU4`, `DlDsyX+XsoA`, `DDQjbwNC31E`) run the same body as their already implemented C1/C2 twin, because `messages` has no virtual base; the console's `libSceLibcInternal.sprx` was not read for them and no title was seen calling them. They write only the vtable and the 64-bit reference count, as the other `messages` constructors do, and the name overloads accept only the names `_Locinfo(const char*)` accepts.
