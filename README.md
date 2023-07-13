# Fifo project written in C/C++

This is a repository regarding the communication between processes on the same machine in UNIX. In this case, we simulate a classic client-server architecture where we have an active server(mainServer.cpp) and one or more clients (mainClient.cpp) that are waiting their turn to request the operation and get the result from the server. We can have a maximum of 4 clients and 5 processes simultaneously. There are a lot of problems that can appear: busy waiting, race condition are some examples.

I tried to manage the busy waiting with the function sleep, where the client sleep for 10 or more seconds if can't comunicate(only one client a time can "get" the server); for the race condition, a possible solution is the shared memory but isn't enough in a more generic case. The shared memory solution option is a good, and easy, option to implement but also danger. There are better solution like semaphore or something else.

There are other check like the busy status of the server but also the effective status of the server(if he isn't a process executed, the client can't gain the shared memory segment and so the architecture doesn't work).
When the server is killed by a Signal (SIGTERM), all the clients are killed by the pid that are "saved" in both variables and in a queue.
Project written in C++ for the objects.
