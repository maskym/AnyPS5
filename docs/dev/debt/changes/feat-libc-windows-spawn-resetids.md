# libSceLibcInternal

## change [posix_spawn and posix_spawnp]

- [posix_spawn and posix_spawnp](../../../core/libs/prx/libSceLibcInternal/Spawn.cpp) (libSceLibcInternal) have unverified console sandbox and error equivalence. POSIX errors missing from the shared translator fall back to `EIO`. Only converted programs under `/app0/` run (`/app0/eboot.bin` uses the running image). Windows open/duplication actions redirect only descriptors 0 to 2; `POSIX_SPAWN_RESETIDS` does nothing, as guest user and group IDs are always 0; other nonzero attribute flags throw. POSIX hosts implement only `POSIX_SPAWN_RESETIDS` and `POSIX_SPAWN_SETPGROUP`; other flags throw. Guest IDs are host IDs, a null `argv` returns `EFAULT`, and null or uninitialized objects throw where FreeBSD dereferences them. A null `envp` inherits the parent environment. `posix_spawnp` follows FreeBSD 11 `_execvpe` path search; PPSA99305 reaches it while SDL searches for `zenity`
