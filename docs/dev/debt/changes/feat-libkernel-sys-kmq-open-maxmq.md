# libkernel_sys

## change [kmq_open, kmq_unlink, kmq_setattr, kmq_timedsend and kmq_timedreceive]

- [kmq_open, kmq_unlink, kmq_setattr, kmq_timedsend and kmq_timedreceive](../../../core/libs/prx/libkernel_sys/Mqueue.cpp) (libkernel_sys) follow FreeBSD 11 `uipc_mqueue.c` with each open queue on a descriptor reserved on the host null device and the names in a table of this process: `mode` is accepted without checking, the owner and permission checks are not modelled, the limit of 100 queues from FreeBSD's `kern.mqueue.maxmq` that makes `kmq_open` fail with `ENFILE` counts only this process's queues where FreeBSD counts the whole system, a queue descriptor cannot be waited on with `select`, `poll` or `kevent`, and `dup` of one does not give a queue
