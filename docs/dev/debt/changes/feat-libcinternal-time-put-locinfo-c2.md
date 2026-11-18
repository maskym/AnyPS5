# libSceLibcInternal

## add Functional

- [time_put C2 from _Locinfo](../../../core/libs/prx/libSceLibcInternal/TimeGetFields.cpp) (libSceLibcInternal, NID `IuFImJ5+kTk`) is unverified on a console: it behaves as the C1 `u5yK3bGG1+w`, so `time_put<char>` fills `0xb8` bytes from `_Getptimes` and does not read the `_Locinfo`. Based on [xloctime](https://github.com/microsoft/STL/blob/770da5aa457af11f0a796600e1aa6f7f6fce14a6/stl/inc/xloctime#L130-L815).

## remove [_ZNSt8time_putIcSt19ostreambuf_iteratorIcSt11char_traitsIcEEEC2ERKSt8_Locinfom]
