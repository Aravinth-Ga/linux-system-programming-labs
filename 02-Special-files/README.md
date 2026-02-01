# Special Files

Devices, pipes/FIFOs, and sockets still use read/write, but they do not behave
like regular files. These are called as special files.
Keeping them separate makes it easier to compare.

Next
- /dev/null, /dev/zero, /dev/tty
- pipes and FIFOs
- AF_UNIX sockets, then TCP/UDP
