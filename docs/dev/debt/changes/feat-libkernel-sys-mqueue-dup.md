# libkernel_sys

## change [kmq_open, kmq_unlink, kmq_setattr, kmq_timedsend and kmq_timedreceive](../../../core/libs/prx/libkernel_sys/Mqueue.cpp)

- [kmq_open, kmq_unlink, kmq_setattr, kmq_timedsend and kmq_timedreceive](../../../core/libs/prx/libkernel_sys/Mqueue.cpp) (libkernel_sys) follow FreeBSD 11 `uipc_mqueue.c` with each open queue on a descriptor reserved on the host null device and the names in a table of this process: `mode` is accepted without checking, the owner and permission checks are not modelled, the per-process queue count is not capped so `kmq_open` never returns `ENFILE` where FreeBSD's `kern.mqueue.maxmq` limits it to 100, and a queue descriptor cannot be waited on with `select`, `poll` or `kevent`
