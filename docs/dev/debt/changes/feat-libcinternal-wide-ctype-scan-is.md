# libSceLibcInternal

## remove _ZNKSt5ctypeIwE10do_scan_isEsPKwS2_

## add Functional

- [std::ctype<wchar_t>::do_scan_is](../../../core/libs/prx/libSceLibcInternal/WideCtype.cpp) (libSceLibcInternal) - NID `pSQz254t3ug` (`_ZNKSt5ctypeIwE10do_scan_isEsPKwS2_`) returns the first character in the 16-bit range for which the facet's `do_is` with the mask is true, or the end, as in [microsoft/STL `xlocale`](https://github.com/microsoft/STL/blob/219514876ea86491de191ceaa88632616bbc0f19/stl/inc/xlocale#L2957-L2965). The STL calls `is`, which is the virtual `do_is`, so the call goes through vtable slot 4, as the measured `do_scan_not` does. The console's `do_scan_is` was not disassembled or measured. A reversed, misaligned or unreadable range throws. Closes when: compared with the module.
