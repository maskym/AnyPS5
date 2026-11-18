# libkernel

## change [sendmsg and recvmsg](../../../core/libs/prx/libkernel/Socket/src/Udp.cpp) (libkernel) pass descriptors

- [sendmsg and recvmsg](../../../core/libs/prx/libkernel/Socket/src/Udp.cpp) (libkernel) pass descriptors in `SCM_RIGHTS` messages on Unix-domain sockets through the host's `SCM_RIGHTS` (Linux only), in FreeBSD's guest layout: a 12-byte `cmsghdr`, data at offset 16, `CMSG_SPACE` records. Several `SCM_RIGHTS` messages in one `sendmsg` arrive as one host-merged record (FreeBSD keeps one per message), so a buffer sized for the first can't receive it alone. A received socket gets default emulated options. Sending a guest kqueue returns `EOPNOTSUPP`, as FreeBSD's `unp_internalize` does; a random device, directory or JIT shared memory descriptor (libkernel's state) throws, as does a zero-byte stream send with descriptors (Linux carries no record without data). Windows checks the buffer, then throws (socket pairs are loopback TCP), as do other control message types and control data on IP sockets. `MSG_PEEK` with a control buffer reads only the data and leaves passed descriptors queued.

## add Functional

- [sendmsg](../../../core/libs/prx/libkernel/Socket/src/Udp.cpp) (libkernel) with control data on Windows fails with `EINVAL` when the control length is outside 12 to 2048 bytes and with `EFAULT` when the control buffer is unreadable, as FreeBSD 11.4's `sendit` and `sockargs` do, then throws: socket pairs are loopback TCP, so `SCM_RIGHTS` descriptors can't be passed
