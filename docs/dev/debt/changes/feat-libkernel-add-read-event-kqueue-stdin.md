# libkernel

## change [sceKernelAddReadEvent on pipes]

- [sceKernelAddReadEvent on pipes, kqueues and standard input](../../../core/libs/prx/libkernel/Equeue/Equeue.cpp) (libkernel) is an exception to the sockets-only rule above: it also takes the read end of a pipe, as `kevent` does through FreeBSD's `filt_piperead`, a kqueue or another equeue's descriptor, as `kevent` does through `filt_kqueue` (see the kevent on a kqueue entry: a cycle of watches throws and the equeue's own descriptor is `EINVAL`), and the host stream on descriptor 0, which never fires, as the kqueue and kevent entry describes; it ignores the size argument on all of them. sceKernelAddWriteEvent on a kqueue is `EINVAL`, as `kevent` refuses every filter but `EVFILT_READ` on a kqueue. Other host streams, other descriptors and a write event on a pipe throw
