# Read

Small exercises around `read()`. Simple ones show return values and EOF;
advanced ones add loops, buffers, and offset checks.

Simple
- `lab01_read_basic.c`
- `lab02_read_return_values.c`

Advanced
- `lab04_read_loop_and_buffer.c`
## Notes: What I Learned About read()

### The Basics
`read()` takes bytes from a file and puts them in a buffer. If we give it a file descriptor, a buffer, and how many bytes we want, it tells us back how many bytes it actually read.

### Understanding Return Values
- **Positive number** = how many bytes we actually got back. Sometimes less than we asked for. This happens when the file is small
- **0** = we hit EOF (end of file). Normal, not an error
- **-1** = error. Check `errno` with `perror()`

### How read() Works
- Starts from current file offset and reads forward
- After each `read()`, the offset automatically moves forward
- Next `read()` starts where the last one ended
- By calling `read()` multiple times, we march through the entire file

### Loops and Buffers
- Buffer size matters. I found 4096 bytes is a good size
- When we need all data from a file, we use a loop:
  - Keep calling `read()` until it returns 0 (EOF)
  - Check for -1 each time to catch errors
  - The offset moves automatically, so the loop just keeps going
- Don't assume `read()` reads everything we asked for

### Important Things
- `read()` can return less than we asked. Always check the return value
- 0 return = EOF, not an error
- Always give `read()` a valid buffer and valid size
- Close the file when done
- File offset is per-process, not shared