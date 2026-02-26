# Read

Small exercises around `read()`. Simple ones show return values and EOF;
advanced ones add loops, buffers, and offset checks.

Simple
- `lab01_read_basic.c`
- `lab02_read_return_values.c`
- `lab03_read_permission_error.c`

Advanced
- `lab04_read_loop_and_buffer.c`
- `lab05_read_filestat_and__offset.c`
## Notes: What I Learned About read()

### The Basics
`read()` takes bytes from a file and puts them in a buffer. If we give it a file descriptor, a buffer, and how many bytes we want, it tells us back how many bytes it actually read.

### Understanding Return Values
- **Positive number** = how many bytes we actually got back. Sometimes less than we asked for. I found this happens when the file is small or the disk is busy
- **0** = we hit EOF (end of file). It means we read past the last byte. This is normal, not an error
- **-1** = something went wrong. We need to check `errno` with `perror()`. I saw errors like EBADF (bad file descriptor), EACCES (no permission), EIO (disk error)

### File Offset
Every open file has an offset. When we call `read()`, it starts from the current offset and moves it forward automatically:
- First time we call `read()` we get bytes 0 to N-1
- Next time we call it, we get bytes N to M-1 (offset already moved!)
- Just by calling `read()` multiple times, we march through the file
- If we need to jump around, we can use `lseek()` to move the offset

### Buffers and Loops
- Buffer size matters a lot for speed. Small buffers are slow (too many syscalls). Bigger buffers are faster
- I saw good sizes like 4096 bytes (one page) or 65536 bytes work well
- When we need all the data from a file, we use a loop:
  - Keep calling `read()` until it returns 0 (EOF)
  - Check for -1 each time to catch errors
  - The offset moves automatically, so we just keep calling `read()`
- One thing I discovered: signals can interrupt `read()`. If we get EINTR, we just call `read()` again

### Important Things
- `read()` can return less than we asked for. I learned not to assume it reads everything in one call
- When `read()` returns 0, that's EOF, not an error. Don't treat it like an error
- We need to always pass a valid buffer and valid size to `read()`
- If a signal interrupts `read()`, we just retry the call
- Permission errors happen if we opened the file read-only or removed read permissions
- I found that the file offset is per-process. Each process has its own offset, not shared