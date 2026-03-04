/*
    Lab 08: Create a pipeline using pipes and exec

    What we're doing:
        1. Create a pipe for communication between two child processes
        2. Fork two child processes
        3. First child: Execute 'cat' command to read file
        4. Second child: Execute 'wc -l' command to count lines
        5. Connect them with a pipe so output of cat goes to input of wc
        6. Parent waits for both children to finish

    System calls used:
        - pipe()      → Create a unidirectional communication channel
        - fork()      → Create child processes
        - dup2()      → Redirect file descriptors (stdout/stdin)
        - close()     → Close file descriptors
        - execlp()    → Replace process with a new program
        - wait()      → Parent waits for child process to finish

    How to compile:
        gcc -Wall -Wextra -O2 -g lab08_pipe_exec_pipeline.c -o lab08

    How to run:
        ./lab08

    What happens:
        - First child process runs: cat lab01_pipe_basic.c
        - Output is piped to second child
        - Second child process runs: wc -l
        - Result is the line count of lab01_pipe_basic.c
 */

#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    // Step 1: Create a pipe for inter-process communication
    int file_descriptor[2];

    if(pipe(file_descriptor) < 0)
    {   
        perror("pipe");
        return 1;
    }
    
    // Step 2: Create first child process
    pid_t pid1 = fork();

    // Child Process 1: Execute 'cat' command (pid1 == 0)
    if(pid1 == 0)
    {
        // Step 3a: Redirect STDOUT to the write end of the pipe
        dup2(file_descriptor[1], STDOUT_FILENO);

        // Step 3b: Close both pipe ends (not needed anymore after dup2)
        close(file_descriptor[1]);
        close(file_descriptor[0]);

        // Step 3c: Replace this process with 'cat' command
        execlp("cat", "cat", "lab01_pipe_basic.c", NULL);

        // Step 3d: This only runs if execlp() FAILS
        perror("execlp");
        exit(EXIT_FAILURE);

    }

    // Step 4: Create second child process
    pid_t pid2 = fork();

    // Child Process 2: Execute 'wc -l' command (pid2 == 0)
    if(pid2 == 0)
    {
        // Step 5a: Redirect STDIN to the read end of the pipe
        dup2(file_descriptor[0], STDIN_FILENO);

        // Step 5b: Close both pipe ends (not needed anymore after dup2)
        close(file_descriptor[0]);
        close(file_descriptor[1]);

        // Step 5c: Replace this process with 'wc -l' command
        execlp("wc", "wc", "-l", NULL);

        // Step 5d: This only runs if execlp() FAILS
        perror("execlp");
        exit(EXIT_FAILURE);

    }

    // Parent Process: Close pipe ends and wait for children
    // Step 6: Close pipe ends in parent process (children have their own copies)
    close(file_descriptor[0]);
    close(file_descriptor[1]);

    // Step 7: Wait for first child to finish
    wait(NULL);

    // Step 8: Wait for second child to finish
    wait(NULL);

    return 0;
}