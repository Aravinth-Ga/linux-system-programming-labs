# Linux System Programming Labs

Notes and small labs from my system programming practice in C. The book
"Linux System Programming" by Robert Love is the starting point, but I also
add topics that are outside the book.

## Layout

- `01-regular-files/` (open/read/write/lseek/stat/permissions/directories/links)
- `02-Special-files/` (dev-null/zero, pipes/FIFOs, sockets)
- `03-process/` (fork/exec/wait/signals)
- `04-memory/` (mmap, shared memory)
- `99-scratch/` (quick tests)

Each folder has a small README with status and a simple/advanced split.

## Concept index

Concept | Regular file | FIFO/pipe | Socket
--- | --- | --- | ---
EOF semantics | yes | yes | tcp-only
lseek() offsets | yes | no | no
Blocking read | yes | yes | yes
Partial read/write | yes | yes | yes
Message boundaries | n/a | no | tcp: no, udp: yes

## Lab naming

Use `labNN_<topic>.c` inside each lab folder, with `NN` local to that folder.

## Current status

**01-regular-files:**
- `open()` - 7 labs (existing file, create, create+exclusive, truncate, append, permission errors)
- `read()` - 5 labs (basic, return values, permission errors, loops, offset with fstat)
- `write()` - 5 labs (basic, with flags, append vs offset, partial writes, fsync/fdatasync)
- `lseek()` - 4 labs (basic, seek end, past EOF, error cases)
- `stat/` - started
- `permissions/` - started
- `directories/` - started
- `links/` - started

**02-Special-files:**
- `pipes/` - 7 basic labs + 3 advanced labs (basic, parent-to-child, child-to-parent, two children, dup2 redirection, dup cooperation, execvp pipeline, nonblocking, error cases)
- `00-dev-null-zero/` - not started
- `01-fifo/` - not started
- `03-sockets/` - not started

**03-process:**
- `01-fork-exec/` - not started
- `02-wait-signals/` - not started

**04-memory:**
- `01-mmap/` - not started
- `02-shm/` - not started

## Next

- Complete `01-regular-files/` (stat, permissions, directories, links)
- Add `/dev/null` and `/dev/zero` labs
- Add FIFO labs
- Add Unix domain socket labs (AF_UNIX)
- Add TCP/UDP socket labs
- Process management (fork/exec/wait/signals)
- Memory management (mmap, shared memory)

## Notes

Build output and quick test files go into `build/` folders and are ignored.
