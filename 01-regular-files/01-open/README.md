# Open

Small experiments around `open()` and flags. Simple are one-shot cases;
advanced show edge cases like O_EXCL and O_APPEND.

Simple
- `lab01_open_existing_read_only.c`
- `lab02_open_nonexistent_no_create.c`
- `lab03_open_create_file.c`
- `lab04_open_permission_denied.c`

Advanced
- `lab05_open_create_exclusive.c`
- `lab06_open_create_truncated.c`
- `lab07_open_append.c`

## Notes: What I Learned About open()

### The Basics
`open()` is how you connect to a file. It gives you a file descriptor (a small number like 3, 4, 5...) that you use for everything else like read, write, close.

### Understanding Flags
Flags tell `open()` what you want to do:
- **O_RDONLY** = read only, can't write
- **O_WRONLY** = write only, can't read  
- **O_RDWR** = read AND write both
- **O_CREAT** = create the file if it doesn't exist. You must give a `mode` parameter
- **O_EXCL** = fail if file already exists. Only works WITH O_CREAT. Useful when you really need a new file, not an old one
- **O_TRUNC** = cut the file down to 0 bytes (empty it). Dangerous! Only works with O_WRONLY or O_RDWR
- **O_APPEND** = always write at the end, no matter where the offset is

### Modes and umask
When you create a file with `open(..., O_CREAT, mode)`:
- `mode` is like 0644 or 0755 (octal numbers)
- But the actual file permission is: `mode & ~umask`
- umask is a mask that blocks certain bits. Default is usually 0022
- So 0666 & ~0022 = 0644 (rw-r--r--)

### Important Things
- Open can fail! Check the return value. -1 means error
- O_EXCL + O_CREAT is atomic - no race condition between check and create
- O_APPEND is reliable for multiple writes, even from different processes
- Permission errors return EACCES or EPERM. Check with `perror()`
- If we don't give O_CREAT a mode, it uses a junk value - always specify mode!
