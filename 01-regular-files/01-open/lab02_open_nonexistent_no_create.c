/*
    Lab 02: Open Non-Existent File without Create Flag

    system calls used:
        - open()
        - close()
    
    Flags used:
      - O_RDONLY

    What this lab teaches:
        - open() returns -1 when trying to open a non-existent file without O_CREAT flag.
        - open does not create a new file if does not exist.
        - close() should not be called if open() fails, since no valid file descriptor is allocated.

    How to compile:
        1. Ensure you are in the directory containing this source file.
        2. Use the following command to compile:
            gcc -Wall -Wextra -O2 -g lab02_open_nonexistent_no_create.c -o lab02
        3. Run the compiled program with a non-existent file:
             ./lab02 nonexistent.txt
*/

#include <fcntl.h>      // For open() and O_RDONLY
#include <stdio.h>      // For printf() and perror()
#include <unistd.h>     // For close()


/*  Main entry point of the program.

    argc : Argument count.
    argv : Argument vector (array of strings).

    Return value : 0 on success, non-zero on failure.
*/
int main(int argc, char* argv[])
{
    const char* file_path = "data_lab02.txt";

    if(argc == 1)
    {
        /* Do not change the file path, Use the predefined one*/
    }
    else if(argc == 2)
    {
        file_path = argv[1];
    }
    else
    {
        fprintf(stderr, "Usage: %s [file]\n", argv[0]);
        return 1;
    }

    printf("Lab02: Attempting to open a non-existent file without O_CREAT flag.\n");

    int fd = open(file_path, O_RDONLY);

    if(fd < 0)
    {
        printf("The open() call returned value : %d on failure\n", fd);
        perror("open");
    }
    
    // This line should not be reached if its reached something worng with the test setup
    printf("[Unexpected] Successfully opened file '%s' with FD: %d\n", file_path, fd);
    // Close the file descriptor if the file is opened successfully
    close(fd);

    return 0;

}
