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
