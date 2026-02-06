/*

    Lab 01: Basic Pipe Example

    System calls used:
        - pipe()

        - read()
        - write()
        - close()

    What this lab teaches:
        - pipe() creates a unidirectional data channel (IPC mechanism).
        - Returns a pair of file descriptors [0] for reading and [1] for writing.
        - Data written to fd[1] can be read from fd[0].
        - Pipes are typically used for inter-process communication between parent and child.


    How to compile:
        1. Ensure you are in the directory containing this source file.
        2. Use the following command to compile:
            gcc -Wall -Wextra -O2 -g lab01_pipe_basic.c -o lab01
        3. Run the compiled program:
             ./lab01

    Hardware tested on:
        - Jetson Orin Nano

    Note : Unnamed pipe is used in this lab.
*/

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


int main(void)
{
    // 1. Create the pipe
    /* fd[0] : read, fd[1] : write*/
    int fd[2];

    int pipe_sts = pipe(fd);

    if(pipe_sts < 0)
    {
        perror("pipe");
        return 1;
    }

    // 2. Write the data into the Pipe

    const char* message = "This is the simple excercise for the pipe.\nThe data written in this buffer will be read on the pipe when the program is executed.\n";
    size_t msg_len = strlen(message);
    size_t total = 0;

    while(total < msg_len)
    {
        ssize_t bytes_written = write(fd[1], message + total, msg_len - total);

        if(bytes_written < 0)
        {
            if(errno == EINTR)
            {
                continue;
            }
            else
            {
                perror("write");
                close(fd[1]);
                return 1;
            }
        }

        total += bytes_written;
    }

    // 3. Read from the pipe.

    // while reading close the write end, Otherwise in the reading it will not detect the EOF
    close(fd[1]);
    char read_buffer[256];

    // reset the values for read operation
    total = 0;
    while(total < sizeof(read_buffer))
    {
        ssize_t bytes_read = read(fd[0], read_buffer + total, sizeof(read_buffer) - 1 - total);

        if(bytes_read < 0)
        {
            if(errno == EINTR)
                continue;
            else
            {
                perror("read");
                close(fd[0]);
                return 1;
            }
        }
        
        if(bytes_read == 0)
        {
            // EOF is reached
            break;
        }

        total += (size_t)bytes_read;
    }
    read_buffer[total] = '\0';

    // 4. Close the file

    if(close(fd[0]) < 0)
    {
        perror("close");
    }

    // 5. printf the final read buffer which is read from the 
    printf("%s", read_buffer);

    return 0;
}