# Linux System Programming Labs

Notes and small labs from my system programming practice in C. The book
"Linux System Programming" by Robert Love is the starting point, but I also
add topics that are outside the book.

## Layout

- `01-regular-files/` (open/read/write/lseek/stat/permissions)
- `02-special-files/` (char devices, pipes, sockets)
- `03-process/` (fork/exec/wait/signals)
- `04-memory/` (mmap, shared memory)
- `99-scratch/` (quick tests)

Each folder has a small README with status and a simple/advanced split.

## Current status

- `open()` basics (existing file, create, create + exclusive, trunc, append)
- Permission errors (`EACCES`)
- `read()` basics (buffer use, return values, loops, empty files)
- File offset checks with `fstat()`

## Next

- Add write() labs
- Add `lseek()` and `stat()` labs
- Add char device labs with `/dev/null` and `/dev/zero`
- Add pipes and FIFOs
- Add Unix sockets (AF_UNIX), then TCP/UDP
- Add process and memory topics when I reach them

## Notes

Build output and quick test files go into `build/` folders and are ignored.
