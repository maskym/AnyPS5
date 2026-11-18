# libkernel_sys

## add Functional

- [kmq_open, kmq_unlink, kmq_timedsend and kmq_timedreceive guest input](../../../core/libs/prx/libkernel_sys/Mqueue.cpp) (libkernel_sys) - the queue name, the sent message and the absolute timeout are copied in with `CopyGuestString_nid_no_patch` and `GuestCopyIn_nid_no_patch`, returning -1 with `EFAULT` (14) while preserving caller `errno` for unreadable memory, with the timeout copied right after the descriptor check as FreeBSD 11 `uipc_mqueue.c` does; this is a host-safety fallback and retail console fault behavior has not been measured.
