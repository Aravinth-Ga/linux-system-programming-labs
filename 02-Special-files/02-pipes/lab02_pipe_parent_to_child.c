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


int main()
{
    int pipefd[2];

    if(pipe(pipefd) == -1)
    {
        perror("pipe");
        return 1;
    
    }

    // fork() creates one child; both processes continue from this line.
    pid_t child_pid = fork();

    if(child_pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if(child_pid == 0)
    {
        /* Child path: fork() returned 0 in the child. */

        // Child only reads from pipefd[0], so close unused write end.

        if(close(pipefd[1]))
        {
            perror("close");
            exit(EXIT_FAILURE);
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
            perror("close");
            exit(EXIT_FAILURE);
        }

        // Child finished reading and can exit.
        exit(EXIT_SUCCESS);

    }
    else
    {
        /* Parent path: fork() returned child PID (> 0). */

        // Parent only writes to pipefd[1], so close unused read end.

        if(close(pipefd[0]) < 0)
        {
            perror("close");
            exit(EXIT_FAILURE);
        }

        const char* plain_text = "Hi! This is the message from parent process.\n";

        ssize_t bytes_written  = write(pipefd[1], plain_text, strlen(plain_text));

        if(bytes_written < 0)
        {
            perror("write");
            exit(EXIT_FAILURE);
        }

        if(close(pipefd[1]) < 0)
        {
            perror("close");
            exit(EXIT_FAILURE);
        }

        // Wait for child to complete before parent exits.
        wait(NULL);

    }


}
