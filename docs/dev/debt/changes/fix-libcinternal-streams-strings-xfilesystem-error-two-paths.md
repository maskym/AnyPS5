# libSceLibcInternal

## remove [_ZNSt10filesystem18_Xfilesystem_errorEPKcRKNS_4pathES4_St10error_code]

## change [_Stat and _Lstat without an error output, _File_size(const char*), _Last_write_time(const char*), _Set_last_write_time, _Rename, _Resize, _Copy_file, _Statvfs and _Current_set2]

- [_Stat and _Lstat without an error output, _File_size(const char*), _Last_write_time(const char*), _Set_last_write_time, _Rename, _Resize, _Copy_file, _Statvfs and _Current_set2](../../../core/libs/prx/libSceLibcInternal/UnversionedFilesystemCalls.cpp) (libSceLibcInternal) - names and NIDs come from the PS5 3.20 export inventory; no module was read and no console measured. The overloads without an error output call this library's overloads with one, passing no output, and `_Last_write_time(const char*)` returns -1 when that call fails and the seconds otherwise. The other six call the matching `std::__sce_v2::filesystem` helpers of libc, which follow the `libc.prx` bundled with PPSA03671 (see [libc](libc.md)), on the assumption that both namespaces build the same Dinkumware sources; so `_Statvfs` returns `ENOSYS` with unknown space and `_Current_set2` accepts only "/". `_Equivalent` and `_Current_set` still throw

## add Functional

- [std::filesystem::_Xfilesystem_error with two paths](../../../core/libs/prx/libSceLibcInternal/CxxRuntimeForwards.cpp) (libSceLibcInternal) - NID `Wut42WAe7Rw` is `tools/nid_names.py` `compute_nid` of `_ZNSt10filesystem18_Xfilesystem_errorEPKcRKNS_4pathES4_St10error_code`, and the export and libc's `_nid_postfix` export both call the shared libc body `FilesystemErrorTwoPaths_nid_no_patch`, so the forward imports no name the patcher turns into its own NID and a Windows import from libSceLibcInternal no longer stops at a stub ahead of libc.prx. The libc body follows the one-path `C6-7Mo5WbwU` read from the SDK 7.00 `libc.prx`; the two-path form, its second path at 0x68 and the null-argument `std::invalid_argument` were not read from the module or run on a console. Closes when: the two-path export is compared with the module
