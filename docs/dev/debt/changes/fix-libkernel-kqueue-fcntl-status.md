# libkernel

## add Functional

- [fcntl on a kqueue or ksem descriptor](../../../core/libs/prx/libkernel/File/src/DescriptorControl.cpp) (libkernel) - `F_SETFL` records `O_NONBLOCK`, `O_APPEND`, `O_ASYNC`, `O_FSYNC`, `O_DIRECT` and the read-ahead flag and then fails with `ENOTTY`, and `F_GETFL` reports `O_RDWR` with the recorded flags, as FreeBSD 11.4's [`kern_fcntl`](https://github.com/freebsd/freebsd-src/blob/release/11.4.0/sys/kern/kern_descrip.c#L542-L566) does: it stores the flags before `fo_ioctl(FIONBIO)`, which [`kqueue_ioctl`](https://github.com/freebsd/freebsd-src/blob/release/11.4.0/sys/kern/kern_event.c#L1895) and the semaphores' [`invfo_ioctl`](https://github.com/freebsd/freebsd-src/blob/release/11.4.0/sys/kern/uipc_sem.c#L142) refuse, and both objects are opened with `FREAD | FWRITE`. The host placeholder beneath them is not changed. Message queue descriptors (libkernel_sys) are not covered. Not measured on a console
