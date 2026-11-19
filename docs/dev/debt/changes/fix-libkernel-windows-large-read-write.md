# libkernel

## add Functional

- [sceKernelRead, sceKernelWrite, read and write](../../../core/libs/prx/libkernel/File/src/Open.cpp) (libkernel) on Windows split a transfer above `INT_MAX` bytes into `INT_MAX` chunks of the UCRT `_read` and `_write`, as FreeBSD 11.4's [`sys_read`/`sys_write`](https://github.com/freebsd/freebsd-src/blob/release/11.4.0/sys/kern/sys_generic.c) accept counts up to `SSIZE_MAX`. Disk-file reads continue until a short chunk, other reads return after the first chunk, and writes continue on every descriptor; a failure after the first chunk returns the bytes already moved. FreeBSD's devfs returns `EINVAL` above `INT_MAX` on a device (`devfs_iosize_max_clamp`); a host console or `NUL` handle here transfers the whole count instead. Not measured on a console.
