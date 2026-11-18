# libSceNKWebKitRequirements

## change [`sqlite3_wal_hook`]

- [`sqlite3_wal_hook`](../../../core/libs/prx/libSceNKWebKitRequirements/src/Sqlite.cpp) returns the host's previous argument, such as the default autocheckpoint frame count, until the guest first calls it.
