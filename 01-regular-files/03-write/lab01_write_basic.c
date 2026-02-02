/*
    Lab 01: Basic write demo

    System calls used:
        open()
        write()
        close()

    Flags used:
        O_WRONLY    (open file in write-only mode)
        O_CREAT     (create file if it doesn't exist)

    Error handling:
        perror() and stderr messages for failures/partial writes.

    What this lab shows:
        writing a short string to a file with write().
*/


#include <fcntl.h>      // for open(), and open flags
#include <stdio.h>      // for printf(), perror()
#include <unistd.h>     // for read(), write(), close()
#include <string.h>     // for strlen()
#include <sys/stat.h>   // for permission flags


/* Main entry point of the program.

    argc : Argument count.
    argv : Argument vector (array of strings).

    Return value : 0 on success, non-zero on failure.
 */

int main(int argc, char* argv[])
{
    const char* file_path = "test_write.txt";

    if(argc == 1)
    {
        /* Use the default file path. */
    }
    else if (argc == 2)
    {
        file_path = argv[1];
    }
    else
    {
        fprintf(stderr, "Usage: %s [file]\n", argv[0]);
        return 1;
    }
    // permission mode and open flag configuration
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int flag = O_WRONLY | O_CREAT | O_TRUNC; 

    // Open the file for writing (create it if missing).
    int fd = open(file_path,flag, mode);

    if(fd < 0)
    {
        perror("open");
        return 1;
    }

    // Write a short message to the file.
    const char message[] = "This is a simple basic write exercise.\n";
    size_t len = strlen(message);

    ssize_t ret = write(fd, message, len);
    
    if(ret >= 0 && ret < (ssize_t)len)
    {
        fprintf(stderr, "Partial write: expected %zd bytes, wrote %zd bytes.\n", len, ret);
        close(fd);
        return 1;
    }
    else if(ret < 0)
    {
        perror("write");
        close(fd);
        return 1;
    }
    else
    {
        printf("Write succeeded: %zd bytes written to %s.\n", ret, file_path);
    }

    if(close(fd) < 0)
    {
        perror("close");
        return 1;
    }

    return 0;
}
