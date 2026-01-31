# Special Files

Pipes, sockets, and device files still use read/write, but they do not behave
like regular files. Keeping them separate makes it easier to compare.

Next
- /dev/null, /dev/zero, /dev/tty
- pipes and FIFOs
- AF_UNIX sockets, then TCP/UDP
