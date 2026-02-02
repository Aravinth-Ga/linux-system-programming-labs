/*
    lab01 : write a file basic demo.

    Systen call used :
        open()
        write()
        close()
    
    Flags used :
        O_WRONLY    (Open file in write-only mode)
        O_CREAT     (Create file if it doesn't exist)
        O_TRUNC     (Truncate file to zero length if it exists)
    
    Error handled :
        perror() usage for error reporting.

    What this lab teaches :
        how to write data to a file using write() syscall.
        
*/


#include <fcntl.h>      // for open(), and open flags
#include <stdio.h>      // for printf(), perror()
#include <unistd.h>     // for read(), write(), close()
#include <string.h>     // for strlen()




int main(int argc, char* argv[])
{
    const char* file_descriptor = "test_write.txt";

    if(argc == 1)
    {
        /* Do not change the file descriptor, use the predefined one. */
    }
    else if (argc == 2)
    {
        file_descriptor = argv[1];
    }
    else
    {
        fprintf(stderr,"Usage: %s [file]", argv[0]);
    }

    // open the file for write
    int fd = open(file_descriptor, O_WRONLY | O_CREAT, 0644);

    if(fd < 0)
    {
        perror("open");
    }

    // Now write some data into the file
    const char buffer[] = "Its a simple basic write excerciese.\n";
    ssize_t len = strlen(buffer);

    ssize_t ret = write(fd, buffer, len);
    
    if(ret > 0 && ret < len)
    {
        fprintf(stderr, "Partial write. Total number of bytes need to write %zd, but written bytes %zd.\n", len, ret);
        return 1;
    }
    else if(ret < 0)
    {
        perror("write");
        return 1;
    }
    else
    {
        // write success close the file
        if(len == ret)
        {
            printf("All the bytes are written and Write success.\n");
            printf("Total number of bytes need to write %zd and written bytes %zd.\n", len, ret);
        }

        if(close(fd)<0)
        {
            perror("close");
            return 1;
        }
    }

    return 0;
}