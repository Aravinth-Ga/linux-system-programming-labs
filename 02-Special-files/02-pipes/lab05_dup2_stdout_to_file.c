/*
    Lab 05: Redirect stdout to file using dup2()

    System calls used:
        - open()
        - dup()
        - dup2()
        - close()

    What this lab teaches:
        - How to redirect file descriptors using dup2().
        - How to redirect stdout to a file.
        - How to save and restore file descriptors.
        - Proper file descriptor management after redirection.

    How to compile:
        1. Ensure you are in the directory containing this source file.
        2. Use the following command to compile:
            gcc -Wall -Wextra -O2 -g lab05_dup2_stdout_to_file.c -o lab05
        3. Run the compiled program:
            ./lab05 <output_file>

    Hardware tested on:
        - Jetson Orin Nano

    Note:
        1. dup2() replaces the file descriptor and closes the old one if already open.
        2. All printf() output will be redirected to the specified file.
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

/*
 * die - Print an error message and terminate the process
 * @msg: Error context string passed to perror()
 *
 * Used for unrecoverable syscall failures in this lab.
 */
// Simple error handler - prints error and exits
static void die(const char* msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}


/*
 * main - Redirect stdout to a file using dup2(), then restore it
 * @argc: Argument count (must be 2)
 * @argv: Argument array (argv[1] is the output filename)
 *
 * Demonstrates file descriptor redirection - stdout goes to file, then terminal.
 * Return: EXIT_SUCCESS on success, EXIT_FAILURE on error
 */
int main(int argc, char* argv[])
{
    // Need exactly one argument: filename
    if(argc != 2)
    {
        fprintf(stderr, "Usage : %s <output_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char* path = argv[1];

    // Open file, create if doesn't exist, truncate if exists
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if(fd < 0)
        die("open");

    // Save original stdout so we can restore it later
    int saved_stdout = dup(STDOUT_FILENO);

    if(saved_stdout < 0)
        die("dup saved_stdout");

    // Redirect stdout to our file
    if(dup2(fd, STDOUT_FILENO) < 0)
        die("dup2");

    // Close original fd - stdout already has it
    if(close(fd) < 0)
        die("close");

    // This goes to the file now (not terminal)
    printf("Line 1 : hello file !\n");
    printf("Line 2 : stdout is redirected using dup2.\n");
    fflush(stdout);  // Make sure it's written before we restore

    // This goes to terminal (stderr not redirected)
    fprintf(stderr,"Printed 2 lines into %s\n", path);

    // Put stdout back to normal (terminal)
    if(dup2(saved_stdout, STDOUT_FILENO) < 0)
        die("dup2 restore stdout");

    // Close backup
    if(close(saved_stdout) < 0)
        die("close saved_stdout");

    // Now printing back to terminal
    printf("back to the terminal : stdout is restored.\n");

    return 0;
}