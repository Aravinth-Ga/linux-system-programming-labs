/*
    lab 07: open a file with permission denied error handling

    syscalls used:
        open()
        write()
        close()
    
    Flags used:
        O_WRONLY    (Open file in write-only mode)
        O_CREAT     (Create file if it does not exist)
        O_EXCL      (Ensure that this call creates the file)
    
*/