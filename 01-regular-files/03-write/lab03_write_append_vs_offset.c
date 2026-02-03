/*
    Lab 03: Write - Append vs Offset

    System Calls used:
        open()
        write()
        lseek()
        close()
    
    Flags used:
        O_WRONLY
        O_CREAT
        O_APPEND

    Error handling:
        perror() and stderr messages for failures.

    What this lab teaches:
        Difference between appending data using O_APPEND flag and using lseek() to set file offset.

*/

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>




int main(int argc, char* argv[])
{
    const char* file_path = "lab03_write.txt";
    int enable_append_mode = 0;

    if(argc == 1)
    {
        /* Do not change the file path, use the predefined one.*/
    }
    else if(argc == 2)
    {
        if(strcmp(argv[1],"append") == 0)
        {
            enable_append_mode = 1;
        }
        else if(strcmp(argv[1],"lseek") == 0)
        {
            enable_append_mode = 0;
        }
        else
        {
            fprintf(stderr,"Usage : %s [append | lseek].\n", argv[0]);
            return 1;
        }
    }
    else if(argc == 3)
    {
        if(strcmp(argv[1], "append") == 0)
        {
            enable_append_mode = 1;
        }
        else if(strcmp(argv[1], "lseek") == 0)
        {
            enable_append_mode = 0;
        }
        else
        {
            fprintf(stderr, "Usage : %s [append | lseek].\n", argv[0]);
            return 1;
        }
        file_path = argv[2];
    }
    else
    {
        fprintf(stderr, "Usage : %s [append | lseek] [file].'\n", argv[0]);
        return 1;
    }

    /* 1. Open a file */

    // Open mode
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    // open flags
    int open_flag = O_WRONLY | O_CREAT;

    if(enable_append_mode == 1)
    {
        open_flag |= O_APPEND;
        printf("Lab03 : File is Opened in Append mode.\n");
    }
    else
    {
        printf("Lab03 : File is Opened in lseek mode.\n");
    }

    // open the file 
    int file_descriptor = open(file_path, open_flag, mode);

    if(file_descriptor < 0)
    {
        perror("open");
        return 1;
    }

    // data which need to be written on the file
    const char buffer[] = "Welcome to the Lab03. This lab shows append vs offset behaviour.\n";

    /* 2. Write the file */

    off_t fd_offset = 0;

    if(enable_append_mode == 0)
    {
        // move the current offset to the EOF if the lseek() is enabled
        fd_offset = lseek(file_descriptor, 0, SEEK_END);

        if(fd_offset < 0)
        {
            perror("lseek");
            close(file_descriptor);
            return 1;
        }

        printf("The current file offset value is : %lld.\n", (long long)fd_offset);        
    }

    size_t length = (size_t)strlen(buffer);
    size_t total = 0;
    ssize_t byte_written = 0;
    while (total < length)
    {
        byte_written = write(file_descriptor, buffer + total, length - total);

        if(byte_written < 0)
        {
            if(errno == EINTR)
            {
                continue;
            }
            else
            {
                perror("write");
                close(file_descriptor);
                return 1;
            }
        }
        
        // This never happens for the regular files, It can happens incase of IO error
        if(byte_written == 0)
        {
            close(file_descriptor);
            return 1;
        }
        
        total += byte_written;    
    }

    fd_offset = lseek(file_descriptor, 0, SEEK_CUR);
    if(fd_offset < 0)
    {
        perror("lseek");
        close(file_descriptor);
        return 1;
    }
    
    printf("The current file offset after write operation is %lld.\n", (long long)fd_offset);
    printf("Number of bytes are written into the fd is %zd and total bytes are %zd.\n", total, length);

    printf("File write is completed.\n");

    /* 3. Close the file */
    if(close(file_descriptor) < 0)
    {
        perror("close");
        return 1;
    }

    return 0;
}