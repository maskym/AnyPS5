# libkernel

## change [pthread_main_np and scePthreadMain]

- [pthread_main_np and scePthreadMain](../../../core/libs/prx/libkernel/Pthread/src/Thread.cpp) (libkernel_sys) - console behaviour is unmeasured. Linux identifies the process initial thread by kernel TID/PID equality, including worker-first loading. Windows takes, at the first query, the live thread of the process with the earliest creation time and keeps a handle to it so its id is not reused; if the initial thread has already exited by then, the oldest remaining thread is taken instead. macOS identifies the thread that initialized libkernel; loading it first from a worker can misidentify the initial thread. The pre-initialization -1 state is not exposed.
