/*
    Lab 10: Pipe error cases and edge conditions

    What we're doing:
        1. Demonstrate parent process forgetting to close write end (EOF hang)
        2. Demonstrate SIGPIPE/EPIPE when writing with no readers
        3. Demonstrate EBADF error when writing to closed file descriptor
        4. Demonstrate proper dup2/exec pipeline implementation

    System calls used:
        - pipe()      → Create a unidirectional communication channel
        - fork()      → Create child processes
        - close()     → Close file descriptors
        - dup2()      → Redirect file descriptors (stdout/stdin)
        - execlp()    → Replace process with a new program
        - write()     → Write data to file descriptor
        - read()      → Read data from file descriptor
        - wait()      → Parent waits for child process to finish

    How to compile:
        gcc -Wall -Wextra -O2 -g lab10_pipe_error_cases.c -o lab10

    How to run:
        ./lab10 <case>
        
        Available cases:
        1: parent forgets to close write end -> read blocks (hang)
        2: child writes but no readers -> SIGPIPE or EPIPE
        3: EBADF by writing to a closed fd
        4: dup2+exec: run "echo hi | wc -c" using a pipe

    What happens:
        - Case 1: Parent process hangs because read() blocks waiting for EOF
        - Case 2: Child process receives SIGPIPE or gets EPIPE error
        - Case 3: write() returns -1 and sets errno to EBADF (Bad file descriptor)
        - Case 4: Proper pipeline works: echo output piped to wc for counting
 */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


static void die(char* msg)
{
    // print error and quit
    perror(msg);
    exit(1);
}

// Display usage instructions for the program
static void usage(char* msg)
{
  fprintf(stderr,
          "Usage: %s <case>\n"
          "  1: parent forgets to close write end -> read blocks (hang)\n"
          "  2: child writes but no readers -> SIGPIPE or EPIPE\n"
          "  3: EBADF by writing to a closed fd\n"
          "  4: dup2+exec: run \"echo hi | wc -c\" using a pipe\n",
          msg);
}

// Error Case 1: Demonstrate EOF hang issue
static void error_case1_eof_hang(void)
{

}

// Error Case 2: Demonstrate SIGPIPE/EPIPE error
static void error_case2_sigpipe_epipe(void)
{

}

// Error Case 3: writing to a closed file descriptor
// try writing after closing the fd - you'll get EBADF (bad file descriptor)
static void error_case3_ebadf(void)
{

}

// Error Case 4: doing it right - create a real pipeline
static void error_case4_dup2_exec(void)
{

}

int main(int argc, char* argv[])
{
    // Validate command line arguments
    if(argc != 2)
        usage(argv[0]);
    
    // get which case to run from command line
    int error_case = atoi(argv[1]);

    switch(error_case)
    {
        case 1:
            // parent doesn't close write end = read never gets EOF = hangs
            error_case1_eof_hang();
            break;
        case 2:
            // trying to write when nobody reads = SIGPIPE (or EPIPE error)
            error_case2_sigpipe_epipe();
            break;
        case 3:
            // write to a closed fd = EBADF
            error_case3_ebadf();
            break;
        case 4:
            // Case 4: Proper dup2/exec pipeline
            error_case4_dup2_exec();
            break;
        default:
            usage(argv[0]);
            break;
    }

    return 0;
}