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
        - The child process writes data to the pipe, and the parent process reads from it.
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
        1. Partial Read/Write is error handling done in this basic lab.
        2. Child Write and Parent read is a design choice.

 */


 #include <fcntl.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <sys/wait.h>
 #include <errno.h>
 #include <string.h>


/*
 * die_pipe - Print an error message and terminate the process
 * @msg: Context string passed to perror()
 *
 * Used for unrecoverable syscall failures in this lab.
 */
 static void die_pipe(const char* msg)
 {
    perror(msg);
    exit(EXIT_FAILURE);
 }

/*
 * main - Demonstrate one-way pipe communication (child -> parent)
 *
 * Flow:
 * 1) Create a pipe
 * 2) Fork into parent and child
 * 3) Child writes a message, parent reads it
 * 4) Close unused ends in each process and wait for child
 *
 * Return: 0 on success
 */
 int main()
 {
    int pipe_fd[2];  // [0] read end, [1] write end

    // Create a unidirectional pipe.
    if(pipe(pipe_fd) < 0)
    {
        die_pipe("pipe");
    }

    // Create child process; both parent and child continue from here.
    pid_t process_id = fork();

    // Handle fork failure.
    if(process_id < 0)
    {
        die_pipe("fork");
    }

    if(process_id == 0)
    {
        // Child is the writer: close unused read end.
        if(close(pipe_fd[0]) < 0)
        {
            die_pipe("child close(pipe_fd[0])");
        }

        // Message sent from child to parent.
        const char* message = "Hello Parent! A message from the child process.\n";
        size_t len = strlen(message);

        // Write message to pipe.
        ssize_t bytes_written = write(pipe_fd[1], message, len);

        // Check write result.
        if(bytes_written < 0)
        {
            die_pipe("child process write");
        }

        // notify if write completed partially (basic handling for this lab).
        if((size_t)bytes_written < len)
        {
            fprintf(stderr,"Child : Partial write (wrote %zd bytes of %zu).\n", bytes_written, len);
        }

        // Close write end so parent can observe EOF.
        if(close(pipe_fd[1])<0)
        {
            die_pipe("child close(pipe_fd[1])");
        }
    }
    else
    {
        // Parent is the reader: close unused write end.
        if(close(pipe_fd[1]) < 0)
        {
            die_pipe("Parent close(pipe_fd[1])");
        }

        // Read one message from child.
        char buffer[256];
        ssize_t bytes_read = read(pipe_fd[0], buffer, sizeof(buffer)-1);

        // Check read result.
        if(bytes_read < 0)
        {
            die_pipe("Parent read");
        }

        // Null-terminate and print what was read.
        buffer[bytes_read] = '\0';
        printf("Parent received the child message : %s\n", buffer);

        // Reader done with the pipe.
        if(close(pipe_fd[0]) < 0)
        {
            die_pipe("parent close(pipe_fd[0])");
        }

        // Reap child process.
        int status = 0;

        if(waitpid(process_id, &status, 0) == -1)
        {
            die_pipe("waitpid");
        }

        // Report child exit status.
        if (WIFEXITED(status)) 
        {
            printf("child exited with status %d \n", WEXITSTATUS(status));
        } else 
        {
            printf("child did not exit normally.\n");
        }

    }

    return 0;

 }
