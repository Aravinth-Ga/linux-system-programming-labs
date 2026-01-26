/*
    lab 07: open a file with permission denied error handling

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



/*  Main entry point of the program

    argc : Argument count
    argv : Argument vector (array of strings)

    Return : 0 on success and non-zero on error
*/

int main(int argc, char* argv[])
{
    const char* file = "lab07_secret.txt";

    
}
