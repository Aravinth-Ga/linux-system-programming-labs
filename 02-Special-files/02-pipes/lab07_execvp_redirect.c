/*
    Lab 07: Redirecting child process STDOUT with fork() and execvp()

    System calls used:
        - fork()
        - execvp()
        - dup2()
        - open()
        - close()
        - wait()

    What this lab teaches:
        - How to fork a child process for executing external commands.
        - How to redirect a child process's standard output to a file using dup2().
        - The relationship between fork, execvp, and file redirection.
        - How execvp() searches for commands in PATH.
        - Proper parent-child process synchronization with wait().

    How to compile:
        1. Ensure you are in the directory containing this source file.
        2. Use the following command to compile:
            gcc -Wall -Wextra -O2 -g lab07_execvp_redirect.c -o lab07
        3. Run the compiled program:
            ./lab07 <command> [args...]
        4. Examples:
            ./lab07 ls -l
            ./lab07 echo "Hello from child"
            ./lab07 cat /etc/hostname

    Hardware tested on:
        - Jetson Orin Nano

    Note:
        1. The child process's STDOUT is redirected to "output.txt" before execvp().
        2. execvp() searches for the command in PATH, similar to shell behavior.
        3. The parent waits for the child to complete and prints the exit status.
        4. File redirection happens transparently to the executed program.
        5. Output from the executed command is saved to output.txt, not displayed on console.
 */

 #include <fcntl.h>
 #include <stdlib.h>
 #include <string.h>
 #include <unistd.h>
 #include <sys/wait.h>
 #include <stdio.h>
 

 int main()
 {
    pid_t pid = fork();

    if(pid == 0)
    {
        int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

        if(fd < 0)
        {
            perror("open");
            exit(EXIT_FAILURE);
        }

        if(dup2(fd, STDOUT_FILENO) < 0)
        {
            perror("dup2");
            close(fd);
            exit(EXIT_FAILURE);
        }

        if(close(fd) < 0)
        {
            perror("close");
            exit(EXIT_FAILURE);
        }

        char *args[] = {"ls", "-lh", NULL};

        execvp(args[0], args);

        perror("execvp");

        exit(EXIT_FAILURE);

    }
    else
    {
        wait(NULL);
        printf("Child finished its execution.!\n");
    }

    return 0;
 }