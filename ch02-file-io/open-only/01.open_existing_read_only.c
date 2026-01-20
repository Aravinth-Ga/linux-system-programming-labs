/*

    Lab 01: Open Existing File in Read-Only Mode

    System calls used:
        - open()
        - close()

    Flags used:
        - O_RDONLY

    What this lab teaches:
        - open() returns a FD (File Descriptor) on success, -1 on failure.
        - This FD is an index to an entry in the per-process file descriptor table.
        - The file descriptor is non-negative integer.
        - No innode (index node) is created when the file is already existing.

    How to compile:
        1. Ensure you are in the directory containing this source file, if not create a 
           file named data.txt using:
             touch data.txt
        2. Use the following command to compile:
             gcc -Wall -Wextra -O2 -g 01_open_existing_read_only.c -o lab01
        3. Run the compiled program:
             ./lab01 data.txt
*/

#include <fcntl.h>      // For open() and O_RDONLY
#include <stdio.h>
#include <unistd.h>



int main(int argc, char* argv[])
{
    // Use data.txt by default, or take the path from the first argument.
    const char* file_path = "data.txt";

    if(argc == 2)
    {
        file_path = argv[1];
    }

    // Tell the user what we are about to do.
    printf("Lab 01: open an existing file in read only mode.\n");
    printf("Calling open(file_path, O_RDONLY [Read only mode])\n");
    
    // Try to open the file in read-only mode.
    int fd = open(file_path, O_RDONLY);

    if(fd < 0)
    {
        perror("open");
        return 1;
    }

    // Show the file descriptor value returned by open().
    printf("The file is opened successfully.\n");
    printf("This is the non negative integer value returned by open() on success : %d\n", fd);

    // Close the file to avoid leaking the descriptor.
    printf("Now call the close(fd) to avoid the FD leaks.\n");
    if(close(fd) < 0)
    {
        perror("close");
        return 1;
    }


    printf("[OK] close() succeeded and Lab 01 is completed.\n");


    return 0;
}
