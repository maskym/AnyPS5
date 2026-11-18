# libkernel

## add Unknown function info

- [getrusage](../../../core/libs/prx/libkernel/System/src/Process.cpp) (libkernel) - on Windows only the user and system times and `ru_maxrss` are filled; `ru_maxrss` is the peak working set of the process in KiB, for the process and for a thread, as the Linux host reports it. `ru_minflt` and `ru_majflt` stay 0 because the Windows page-fault count does not separate soft from hard faults, `ru_inblock` and `ru_oublock` stay 0 because the I/O counters count operations, not blocks, and `ru_nvcsw` and `ru_nivcsw` stay 0 because no Windows context-switch count is read. Closes when: each has a host source
