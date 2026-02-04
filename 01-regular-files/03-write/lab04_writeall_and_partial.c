/*
    Lab 04 - Write All and Partial

    System calls:
        open()
        write()
        close()

    Flags:
        O_WRONLY
        O_CREAT
        O_APPEND

    What this lab shows:
        - write() can write fewer bytes than requested.
        - Keep writing in a loop until all bytes are written.
        - Always check return values for errors.
*/

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char* argv[])
{
    const char* file_path = "lab04_write_all_partial.txt";

    if(argc == 1)
    {
        /* Do not change the file path, use the predefined one. */
    }
    else if(argc == 2)
    {
        file_path = argv[1];
    }
    else
    {
        fprintf(stderr, "Usage : %s [file].\n", argv[0]);
        return 1;
    }

    // 1) Open the file for writing

    // File permissions if the file is created
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    // Open flags
    int flag = O_WRONLY | O_CREAT | O_APPEND;

    int fd = open(file_path, flag, mode);
    
    if(fd < 0)
    {
        perror("open");
        return 1;
    }

    // 2) Write data to the file

    // Message to write
    const char buffer_loc[] = "This lab explains write() and how to handle partial writes.\n";
    size_t length = strlen(buffer_loc);
    size_t total_bytes_written = 0;
    int retry_count = 0;

    while(total_bytes_written < length)
    {
        ssize_t bytes_written = write(fd, buffer_loc + total_bytes_written, length - total_bytes_written);

        if(bytes_written < 0)
        {
            if(errno == EINTR)
            {
                continue;
            }
            else if(errno == EAGAIN)
            {
                sleep(5);
                retry_count++;

                if(retry_count > 10)
                {
                    perror("write");
                    close(fd);
                    return 1;
                }
                continue;
            }
            else
            {
                perror("write");
                close(fd);
                return 1;
            }
        }
        else if(bytes_written == 0)
        {
            perror("write");
            close(fd);
            return 1;
        }
        else
        {
            total_bytes_written += bytes_written;
        }
    }

    // 3) Close the file

    if(close(fd) < 0)
    {
        perror("close");
    }

    return 0;
}
