/*
    Lab 02: Write with open flags

    System Calls used:
        open()
        write()
        close()
    
    Flags used:
        O_WRONLY
        O_CREAT
        O_TRUNC
        O_APPEND
        O_EXCL

    Error handling:
        perror() and stderr messages for failures/partial writes.
    
    What this lab shows:
        how open flags change the behavior of write() system call.

*/

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>




int main(int argc, char* argv[])
{
    const char* file_path = "test_write.txt";

    if(argc == 1)
    {
        /* Do not change the file path, use the predefined one. */
    }
    else if (argc == 2)
    {
        file_path = argv[1];
    }
    else
    {
        fprintf(stderr,"Usage: %s [file]", argv[0]);
        return 1;
    }

    // open the file
    mode_t open_mode = S_IRGRP | S_IROTH | S_IRUSR | S_IWUSR;
    int open_flag = O_WRONLY | O_CREAT | O_APPEND;

    int fd = open(file_path, open_flag, open_mode);

    if(fd < 0)
    {
        perror("open");
    }

    // write the file
    const char buffer[] = "Write with open flags are practiced in this lab.\n";
    size_t len = strlen(buffer);

    ssize_t ret = write(fd, buffer, len);

    if(ret < 0)
    {
        perror("write");
        close(fd);
        return 1;
    }

    if(ret != (ssize_t)len)
    {
        fprintf(stderr, "Partial write. Number of bytes %zd is expected, but wrote %zd.\n",len, ret);
        close(fd);
        return 1;
    }
    else
    {
        printf("Write success.\n");
    }

    return 0;
}