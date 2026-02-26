# Pipes

Small exercises around `pipe()`. Simple labs cover creating pipes and
parent/child communication; advanced labs cover `dup2()` with `exec()`
and nonblocking/error behavior.

Simple
- `lab01_pipe_basic.c`
- `lab02_pipe_parent_to_child.c`
- `lab03_pipe_child_to_parent.c`
- `lab04_pipe_two_children.c`
- `lab05_dup2_stdout_to_file.c`
- `lab06_dup_shared_offset.c`
- `lab07_execvp_redirect.c`

Advanced
- `lab08_pipe_exec_pipeline.c`
- `lab09_pipe_nonblocking.c`
- `lab10_pipe_error_cases.c`
## Notes: What I Learned About Pipes

### The Basics
A pipe is a one-way communication channel between processes. `pipe()` creates two file descriptors - one for reading and one for writing. Data we write on one end comes out on the other end. It's like a tube with input and output.

### How pipe() Works
- `pipe()` takes an array of 2 file descriptors and fills it in
- `fd[0]` = read end of the pipe
- `fd[1]` = write end of the pipe
- Data flows one way only: write to fd[1], read from fd[0]
- `pipe()` returns 0 on success, -1 on failure (usually EMFILE - too many open files)

### Parent to Child Communication
When we fork after creating a pipe:
- Both parent and child inherit copies of fd[0] and fd[1]
- If parent writes, child can read
- Important: close what we don't use! If parent writes, close its read end (fd[0])
- We close unused ends because the pipe stays open as long as ANY process has it open
- EOF only happens when ALL writers close their copies

### Child to Parent Communication
Same idea but reversed:
- Child writes, parent reads
- Parent still needs to close its write end (fd[1])
- Parent closes read end after getting all data
- Child closes both ends after writing (it's done with pipe)

### Multiple Processes with Pipes
I found that pipes work between siblings too:
- Parent creates pipe BEFORE forking
- Parent forks multiple children
- Each child gets the pipe file descriptors
- All children write or all read from same pipe
- Important: each process needs to close unused ends

### dup2() and Redirection
`dup2()` duplicates a file descriptor:
- `dup2(fd[1], STDOUT_FILENO)` makes stdout point to the pipe write end
- Now `printf()` and `write(STDOUT_FILENO)` go to the pipe automatically
- Very useful when combined with `exec()` - the new program inherits redirected stdout/stdin

### Combining with exec()
When we `execvp()` after `dup2()`:
- The executed program inherits the file descriptors
- Its stdout/stdin are already redirected
- The program doesn't need to know about pipes - it just uses normal I/O
- This is how shell pipes work: `cat file | grep pattern`

### File Offset and Pipes
Pipes don't have offsets like files:
- Every read gets fresh data (no seeking backwards)
- Multiple readers on same pipe share the data (each byte goes to one reader)
- Data is consumed as it's read - can't read it again

### Blocking and Nonblocking Behavior
- **Blocking** (default): 
  - Write blocks if pipe is full
  - Read blocks if pipe is empty
  - This is what we usually want
- **Nonblocking** (with fcntl and O_NONBLOCK):
  - Write returns -1 with EAGAIN if pipe is full
  - Read returns -1 with EAGAIN if pipe is empty
  - Useful for handling multiple pipes without waiting

### Error Cases and Edge Cases
- **Write to pipe with no readers**: Get signal SIGPIPE, write returns -1 with EPIPE
- **Read from pipe with no writers**: Returns 0 (EOF) when pipe is empty
- **Broken pipe**: If reader closes fd[0] and we keep writing, EPIPE happens
- **Too many file descriptors**: `pipe()` fails with EMFILE

### Important Things
- Always close unused file descriptors. This is critical for pipes!
- Pipe is usually small (65536 bytes on Linux). Large writes can block
- Don't forget to close write end before reading EOF - the reader will block forever waiting for more data
- Multiple readers on same pipe: each byte goes to ONE reader, not all
- Pipes are unidirectional. For two-way communication, we need TWO pipes
- Signals can interrupt read/write. If EINTR, just retry
- Pipes work between unrelated processes too, not just parent-child (through inheritance)