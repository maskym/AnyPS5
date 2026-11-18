# libc

## add Functional

- [Windows crash report](../../../core/libs/prx/libc/src/specifics/windows/CrashReport.cpp) (libc) - a thread that calls `abort()` while another thread writes the abort report waits for it at most 2 seconds. The UCRT resets the `SIGABRT` handler to `SIG_DFL` before calling it and the handler installs itself again on entry, so an `abort()` landing between the two still ends the process without waiting and can cut the report short.
