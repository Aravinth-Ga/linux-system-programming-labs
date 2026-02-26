# Pipes

Small exercises around `pipe()`. Labs cover creating pipes,
parent/child communication, and redirecting I/O with `dup2()`.

- `lab01_pipe_basic.c`
- `lab02_pipe_parent_to_child.c`
- `lab03_pipe_child_to_parent.c`
- `lab04_pipe_two_children.c`
- `lab05_dup2_stdout_to_file.c`
- `lab06_dup_shared_offset.c`
- `lab07_execvp_redirect.c`

## Notes: What I Learned About Pipes

### The Basics
A pipe is a one-way communication channel between processes. `pipe()` creates two file descriptors - one for reading and one for writing. Data written to one end comes out the other end.

### How pipe() Works
```c
int fd[2];
pipe(fd);
```
- `fd[0]` = read end of the pipe
- `fd[1]` = write end of the pipe
- Data flows one way only: write to fd[1], read from fd[0]
- Returns 0 on success, -1 on failure

### Parent to Child Communication
When we fork after creating a pipe:
- Both parent and child inherit copies of fd[0] and fd[1]
- If parent writes, child reads
- Important: close what we don't use! Close fd[0] if we're writing
- Close unused ends because the pipe stays open as long as ANY process has it open
- EOF happens when ALL writers close their copies

### Child to Parent Communication
Same idea reversed:
- Child writes, parent reads
- Parent closes fd[1] (write end) - it doesn't write
- Parent closes fd[0] after reading all data
- Child closes both ends after writing

### Multiple Children with Same Pipe
- Parent creates pipe BEFORE forking
- Parent forks multiple children
- Each child gets the pipe file descriptors
- All children can write to same pipe (parent reads from all)
- Each process must close unused ends

### dup2() and Redirection
`dup2()` makes one file descriptor point to another:
- `dup2(fd[1], STDOUT_FILENO)` = make stdout point to pipe write end
- After this, `printf()` goes to the pipe automatically
- Very useful for redirecting a program's output without changing the program

### Combining with exec()
When we `execvp()` after `dup2()`:
- The new program inherits ALL file descriptors including redirected ones
- Its stdout is already pointing to our pipe
- The program doesn't know about pipes - just uses normal `printf()` and `write()`

### Important Things
- Always close unused file descriptors. Critical for pipes!
- Must close write end before trying to read EOF - otherwise reader blocks forever
- Multiple readers on same pipe: each byte goes to ONE reader, not broadcast to all
- Pipes are unidirectional. For two-way communication need TWO pipes
- Both parent and child start with same fd values, so both can read/write before closing
- Pipe size is usually 65536 bytes. Large writes can block
- Close inherited fds after fork if child doesn't need them

