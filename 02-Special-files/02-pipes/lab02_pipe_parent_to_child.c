/*
    Lab 02: Pipes parent to child

    System calls used:
        - pipe()
        - fork()
        - read()
        - write()
        - close()
    
    What this lab teaches:
        - How to use pipes for communication between parent and child processes.
        - The parent process writes data to the pipe, and the child process reads from it.
        - Proper handling of file descriptors in both parent and child processes.

    How to compile:
        1. Ensure you are in the directory containing this source file.
        2. Use the following command to compile:
            gcc -Wall -Wextra -O2 -g lab02_pipe_parent_to_child.c
        3. Run the compiled program:
             ./lab02
    
    Hardware tested on:
        - Jetson Orin Nano

    Note : 
        1. Partial Read/Write is not handled in this basic lab.
        2. Child read and Parent write is a design choice and it can be any.

 */

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

/*
 * die_pipe - Print an error message and terminate the process
 * @msg: Short context string passed to perror()
 *
 * Used for unrecoverable syscall failures in this lab.
 */
static void die_pipe(const char* msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}


/*
 * main - Demonstrate one-way pipe communication (parent -> child)
 *
 * Flow:
 * 1) Create a pipe
 * 2) Fork into parent and child
 * 3) Parent writes a message, child reads it
 * 4) Close unused ends in each process and wait for child
 */
int main()
{
    int pipefd[2];

    if(pipe(pipefd) == -1)
    {
        die_pipe("pipe");
        return 1;
    
    }

    // Create a child process; both parent and child continue from here.
    pid_t child_pid = fork();

    if(child_pid < 0)
    {
        die_pipe("fork");
    }
    else if(child_pid == 0)
    {
        /* Child path: fork() returned 0 in the child. */

        // Child is the reader: close the write end it does not use.

        if(close(pipefd[1]))
        {
            die_pipe("close");
        }

        char read_buff[1024];

        ssize_t bytes_read = read(pipefd[0], read_buff, sizeof(read_buff)-1);

        if(bytes_read > 0)
        {
            read_buff[bytes_read] = '\0';
            printf("Child is received : %s\n", read_buff);
        }

        if(close(pipefd[0]) < 0)
        {
            die_pipe("close");
        }

        // Child work is done after reading once.
        exit(EXIT_SUCCESS);

    }
    else
    {
        /* Parent path: fork() returned child PID (> 0). */

        // Parent is the writer: close the read end it does not use.

        if(close(pipefd[0]) < 0)
        {
            die_pipe("close");
        }

        const char* plain_text = "Hi! This is the message from parent process.\n";

        ssize_t bytes_written  = write(pipefd[1], plain_text, strlen(plain_text));

        if(bytes_written < 0)
        {
            die_pipe("write");
        }

        if(close(pipefd[1]) < 0)
        {
            die_pipe("close");
        }

        // Wait so parent does not exit before child finishes.
        wait(NULL);

    }

}
