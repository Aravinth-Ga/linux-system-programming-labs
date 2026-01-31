/*
    lab 04: open a file with permission denied error handling

    syscalls used:
        open()
        write()
        close()
    
    Flags used:
        O_WRONLY    (Open file in write-only mode)
        O_RDONLY    (Open file in read-only mode)
        O_RDWR    (Open file in read-write mode)

    Error handled:
        Permission denied
    
    What this lab teaches:
        how the system behaves when opening a file without sufficient permissions.
*/

#include <fcntl.h>      // For open(), O_RDONLY, O_WRONLY
#include <unistd.h>     // close()
#include <stdio.h>      // For printf(), perror()
#include <errno.h>
#include <string.h>

// Function prototype
static void file_open(const char* file, int flag, const char* label);

/*  Main entry point of the program

    argc : Argument count
    argv : Argument vector (array of strings)

    Return : 0 on success and non-zero on error
*/

int main(int argc, char* argv[])
{
    const char* file = "lab04_secret.txt";

    // Handle command line arguments
    if(argc == 1)
    {
        /* Do not change the file path, use the predefined one. */
    }
    else if (argc == 2)
    {
        file = argv[1];
    }
    else
    {
        // Invalid arguments
        fprintf(stderr, "Usage : %s [file].\n", file);
        return 1;
    }

    printf("Lab04 : open() permission denied demo.\n");
    printf("Expected setup : chmod 000 filename (therfore the open/read/write fails).\n");

    // Try to open the file with different access modes
    file_open(file, O_RDONLY, "Open as Read only");
    file_open(file, O_WRONLY, "Open as Write only");
    file_open(file, O_RDWR, "Open with Read/Write access");

    return 0;
    
}

/*  Function to open a file with specified flags and to handle the permission denied error.

    file : Path of the file to be opened
    flag : Flags for opening the file 
    label : Label to identify the operation in output

    Return : void
*/
static void file_open(const char* file, int flag, const char* label)
{
    errno = 0;
    // Try to open the file
    int fd = open(file, flag);

    if(fd < 0)
    {
        // Open failed, print the error
        printf("[Fail] %s: open(\"%s\") returned -1, errno=%d (%s).\n", label, file, errno, strerror(errno)); 
        return;
    }

    printf("[OK] The file open is success.\n");

    if(close(fd)< 0)
    {
        perror("close");
        return;
    }

    printf("[Closed] The file is closed successfully!\n");

}
