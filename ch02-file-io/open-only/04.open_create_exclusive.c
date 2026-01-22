/*
    Lab 04 : Create a file using open() with O_CREAT | O_EXCL flags

    System calls used:
        - open()
        - write()
        - close()
    
    Flags used:
        - O_CREAT   (Create file if it does not exist)
        - O_EXCL    (Ensure that this call creates the file, fail if it already exists)
        - O_WRONLY  (Open file in write-only mode)

    Mode (permission bits) used (only when creating a new file):
        - 0644 : owner can read/write, group can read, others can read.

    What this lab teaches:
        - O_CREAT | O_EXCL create a new file only if does not already exist.
        - File creation is atomic safe (race-condition safe) when using O_CREAT | O_EXCL.
        - if the file exist, open() fails with EEXIST error.
*/

#include <fcntl.h>    // For open(), O_CREAT, O_EXCEL, O_WRONLY
#include <stdio.h>    // For printf() and perror()
#include <unistd.h>   // For close()
#include <sys/stat.h> // For mode constants


/*  Main entry point of the program.

    argc : Argument count.
    argv : Argument vector (array of strings).

    Return value : 0 on success, non-zero on failure.
*/
int main(int argc, char* argv[])
{
    const char* file_name = "data_lab04.txt";

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

    printf("Lab04: Attempting to create a new file with O_CREAT | O_EXCL flags (atomic create).\n");
    printf("Target file path is: %s\n", file_name);

    // Set flags for open() call 
    // O_WRONLY : Open file in write-only mode
    // O_CREAT  : Create file if it does not exist
    // O_EXCL   : Ensure that this call creates the file, fail if it already exists
    int flags = O_WRONLY | O_CREAT | O_EXCL;

    // Set mode (permission bits) for the new file
    // S_IRUSR : Read permission for owner
    // S_IWUSR : Write permission for owner
    // S_IRGRP : Read permission for group
    // S_IROTH : Read permission for others
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH; // 0644

    // Call open() to create and open the file
    int fd = open(file_name, flags, mode);

    // Check for errors
    if(fd < 0)
    {
        // open() failed
        printf("The open() call returned value : %d on failure\n", fd);
        perror("open");
        return 1;
    }

    printf("The file is created and opened successfully.\n");
    printf("This is nonnegative integer value returned by open() on success : %d\n", fd);
    
    
    printf("Now writing some data to the newly created file...\n");

    const char data[] = "Hello from lab04. \n";

    // some times here the data could be written partially.This will be handled in later labs.
    if(write(fd, data, sizeof(data)-1) < 0)
    {
        // write() failed
        perror("write");
        close(fd); // Close the file descriptor before exiting
        return 1;
    }
    
    printf("Data written successfully to the file.\n");
    
    printf("Now call the close(fd) to avoid the FD leaks.\n");

    // Close the file descriptor to avoid FD leaks
    int fdc = close(fd);
    if(fdc < 0)
    {
        // close() failed
        perror("close");
        return 1;
    }

    printf("[OK] File descriptor %d closed successfully.\n", fd);
    printf("Now, you can verify the file creation using 'ls -l %s' command.\n", file_name);
    
    return 0;
}