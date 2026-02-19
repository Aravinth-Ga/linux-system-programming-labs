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
 * die_pipe - Print error message and exit the program
 * @msg: Error message to print using perror()
 * 
 * This utility function prints the provided error message along with
 * the system error description and then terminates the program with EXIT_FAILURE.
 */
 static void die_pipe(const char* msg)
 {
    perror(msg);  // Print error message
    exit(EXIT_FAILURE);  // Exit with failure status
 }

/*
 * main - Demonstrates pipe communication where child writes to parent
 * 
 * Creates a pipe and forks a child process. The child writes a message to the pipe
 * while the parent reads from it. Shows proper file descriptor management and IPC.
 * 
 * Return: 0 on success
 */
 int main()
 {
    int pipe_fd[2];  // File descriptors: [0] = read end, [1] = write end

    // Create a unidirectional pipe for parent-child communication
    if(pipe(pipe_fd) < 0)
    {
        die_pipe("pipe");
    }

    // Fork a new process for child-parent communication
    pid_t process_id = fork();

    // Check for fork error
    if(process_id < 0)
    {
        die_pipe("fork");
    }

    if(process_id == 0)
    {
        // ============== CHILD PROCESS ==============
        // Child writes a message through the pipe to the parent process
        // First, close the read end since child only writes
        if(close(pipe_fd[0]) < 0)
        {
            die_pipe("child close(pipe_fd[0])");
        }

        // Prepare the message to send to parent
        const char* message = "Hello Parent! A message from the child process.\n";
        size_t len = strlen(message);

        // Write message to the pipe
        ssize_t bytes_written = write(pipe_fd[1], message, len);

        // Check for write errors
        if(bytes_written < 0)
        {
            die_pipe("child process write");
        }

        // Warn if partial write occurred
        if((size_t)bytes_written < len)
        {
            fprintf(stderr,"Child : Partial write (wrote %zd bytes of %zu).\n", bytes_written, len);
        }

        // Close the write end to signal EOF to parent
        if(close(pipe_fd[1])<0)
        {
            die_pipe("child close(pipe_fd[1])");
        }
    }
    else
    {
        // ============== PARENT PROCESS ==============
        // Parent reads the message written by child through the pipe
        // First, close the write end since parent only reads
        if(close(pipe_fd[1]) < 0)
        {
            die_pipe("Parent close(pipe_fd[1])");
        }

        // Read message from the pipe
        char buffer[256];
        ssize_t bytes_read = read(pipe_fd[0], buffer, sizeof(buffer)-1);

        // Check for read errors
        if(bytes_read < 0)
        {
            die_pipe("Parent read");
        }

        // Null-terminate the message and display it
        buffer[bytes_read] = '\0';
        printf("Parent received the child message : %s\n", buffer);

        // Close the read end after communication is complete
        if(close(pipe_fd[0]) < 0)
        {
            die_pipe("parent close(pipe_fd[0])");
        }

        // Wait for child process to complete
        int status = 0;

        if(waitpid(process_id, &status, 0) == -1)
        {
            die_pipe("waitpid");
        }

        // Check how the child process exited
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