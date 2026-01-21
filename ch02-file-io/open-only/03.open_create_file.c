/*
    Lab 03: Open Non-Existent File with Create Flag

    System calls used:
        - open()
        - close()
    
    Flags used:
        - O_CREAT
        - O_WRONLY
    
    Mode (permission bits) used (only when creating a new file):
        - 0644 : owner can read/write, group can read, others can read.

    What this lab teaches:
        - open() create a real file (innode (index node) + directory entry) when O_CREAT flag is used.
        - The mode parameter is used only when the O_CREAT flag is used.
        - The final permission bits are reduced by the umask (user file-creation mask).

*/     

#include <fcntl.h>      // For open(), O_CREAT, O_WRONLY
#include <stdio.h>      // For printf() and perror()
#include <unistd.h>     // For close()


int main(int argc, char* argv[])
{
    const char* file_name = "data_lab03.txt";

    if(argc == 1)
    {
        /* Do not change the file path, Use the predefined one*/
    }
    else if(argc == 2)
    {
        file_name = argv[1];
    }
    else
    {
        fprintf(stderr, "Usage: %s [file]\n", argv[0]);
        return 1;
    }

    printf("Lab03: Attempting to open a non-existent file with O_CREAT flag.\n");

    int fd = open(file_name, O_RDONLY | O_CREAT, 0644);

    if(fd < 0)
    {
        printf("The open() call returned value : %d on failure\n", fd);
        perror("open");
        return 1;
    }

    printf("Successfully opened (and created if not existing) file '%s' with FD: %d\n", file_name, fd);

    // Close the file descriptor
    close(fd);
}