/*
    Lab 07: Run a command in a child process and save its output to a file

    What we're doing:
        1. Create a child process using fork()
        2. Redirect the child's output to a file using dup2()
        3. Run a command (like 'ls') in the child using execvp()
        4. Parent waits for child to finish using wait()

    System calls used:
        - fork()      → Create a copy of the parent process
        - execvp()    → Replace child with a new program
        - dup2()      → Redirect output to a file
        - open()      → Create/open a file
        - close()     → Close file descriptor
        - wait()      → Parent waits for child to finish

    How to compile:
        gcc -Wall -Wextra -O2 -g lab07_execvp_redirect.c -o lab07

    How to run:
        ./lab07

    What happens:
        - Child process runs: ls -lh
        - Output is saved to: output.txt
        - Parent prints: "Child finished its execution.!"
 */

 #include <fcntl.h>
 #include <stdlib.h>
 #include <string.h>
 #include <unistd.h>
 #include <sys/wait.h>
 #include <stdio.h>
 

 int main()
 {
    // Step 1: Create a child process
    // fork() returns 0 in child, child's PID in parent
    pid_t pid = fork();

    //  Child Process (pid == 0) 
    if(pid == 0)
    {
        // Step 2a: Create (or open) a file for saving output
        int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

        // Check if file opening failed
        if(fd < 0)
        {
            perror("open");  // Print error message
            exit(EXIT_FAILURE);
        }

        // Step 2b: Redirect STDOUT to the file
        if(dup2(fd, STDOUT_FILENO) < 0)
        {
            perror("dup2");
            close(fd);
            exit(EXIT_FAILURE);
        }

        // Step 2c: Close the file descriptor
        if(close(fd) < 0)
        {
            perror("close");
            exit(EXIT_FAILURE);
        }

        // Step 3: Prepare the command to run
        char *args[] = {"ls", "-lh", NULL};

        // Step 4: Replace this child process with the 'ls' program
        execvp(args[0], args);

        // Step 5: This only runs if execvp() FAILS.
        perror("execvp");

        exit(EXIT_FAILURE);

    }
    // Parent Process (pid > 0)
    else
    {
        // Step 6: Wait for child process to finish.
        wait(NULL);

        // Step 7: Child finished.
        printf("Child finished its execution.!\n");
    }

    return 0;
 }