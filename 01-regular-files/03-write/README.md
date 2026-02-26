# Write

Small exercises around `write()`. Simple ones cover basics and flags;
advanced ones explore append vs offsets, partial writes, and durability.

Simple
- `lab01_write_basic.c`
- `lab02_write_with_open_flags.c`
- `lab03_write_append_vs_offset.c`

Advanced
- `lab04_writeall_and_partial.c`
- `lab05_write_durability_fsync_fdatasync.c`
## Notes: What I Learned About write()

### The Basics
`write()` sends bytes from our buffer to a file. We give it a file descriptor, a buffer with data, and how many bytes we want to send. It tells us back how many bytes it actually wrote.

### Return Values
- **Positive number** = how many bytes we actually wrote. Sometimes less than we asked for! This surprised me at first
- **0** = nothing was written. This is rare but can happen
- **-1** = error. We check `errno` with `perror()`. Errors like EBADF (bad fd), EACCES (no permission), ENOSPC (disk full)

### File Offset and Position
When we write, the offset moves forward automatically:
- First `write()` puts data at the start (offset 0)
- Next `write()` puts data right where the first one ended (offset moved!)
- The offset keeps moving forward with each write
- With O_APPEND flag, write always goes at the end, no matter what the offset is

### Flags Matter
- **O_WRONLY vs O_RDWR** = must have write permission. Without O_WRONLY or O_RDWR, write fails
- **O_CREAT** = create file if it doesn't exist. Remember to give a mode
- **O_TRUNC** = empty the file first. Dangerous! Only use when we really want to start fresh
- **O_APPEND** = always write at the end. Safe for multiple processes writing to same file

### Partial Writes Are Real
- `write()` can return less than we asked! Don't assume it writes everything in one call
- I discovered we need a loop for reliability:
  - Call `write()` with our data
  - Check return value
  - If less than asked, write the remaining bytes
  - Keep going until everything is written
- This is important for large buffers or slow disks

### Durability and Fsync
- By default, `write()` puts data in a buffer. It doesn't guarantee it's on disk
- `fsync()` = force all data AND metadata to disk. It waits, so it's slow
- `fdatasync()` = force data to disk only, skip metadata. Faster than fsync
- Without fsync or fdatasync, data can be lost if power fails or system crashes
- I learned we need fsync only for critical data. Most of the time it's not needed

### Important Things
- `write()` can write less than we asked. Always check return value
- O_APPEND is atomic for single writes, but not for multiple writes in a loop
- Empty files need a mode parameter with O_CREAT. Forgetting this causes problems
- Signals can interrupt `write()` too. If we get EINTR, just retry
- Disk can run out of space. Check for ENOSPC error
- Multiple processes can write to same file. O_APPEND is safer than manual offsets