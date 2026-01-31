/*
    lab06 : Create and open a file. If the file already exists, truncate its length to zero.

    System calls used:
        - open()
        - write()
        - close()

    Flags used:
        - O_CREAT     (Create file if it does not exist)
        - O_TRUNC     (If the file exists, truncate its length to zero)
        - O_WRONLY    (Open file in write-only mode)

    Mode (permission bits) used (only when creating a new file):
        - 0644 : owner can read/write, group can read, others can read.

    What this lab teaches:
        - O_CREAT | O_TRUNC create a new file if it does not exist, or
          truncate the file to zero length if it already exists.
    
        - umask effects the permission bits of the newly created file.
*/


#include <fcntl.h>    // For open(), O_CREAT, O_TRUNC, O_WRONLY
#include <stdio.h>    // For printf() and perror()
#include <unistd.h>   // For close()
#include <sys/stat.h> // For mode constants



static void print_mode_bits(mode_t mode);

/*  Main entry point of the program.

    argc : Argument count.
    argv : Argument vector (array of strings).

    Return value : 0 on success, non-zero on failure.
*/
int main(int argc, char* argv[])
{
    const char* file_path = "data_lab06.txt";

    if(argc == 1)
    {
        /* Do not change the file path, Use the predefined one*/
    }
    else if (argc == 2)
    {
        file_path = argv[1];
    }
    else{
        fprintf(stderr, "Usage: %s [file]\n", argv[0]);
        return 2;
    }

    printf("Lab06: Attempting to create/open a file withh O_CREAT | O_TRUNC flags.\n");
    printf("target file path is: %s\n", file_path);

    // Set flags for open() call
    int flags = O_WRONLY | O_CREAT | O_TRUNC;
    // Set mode (permission bits) for the new file if created
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    int fd = open(file_path, flags, mode);

    if(fd < 0)
    {
        printf("The open() call returned value : %d on failure\n", fd);
        perror("open");
        return 1;
    }

    // Write a sample message to the file
    const char* message = "Hello world! This is lab06.\n";

    ssize_t bytes_written = write(fd, message, sizeof(message) - 1); // sizeof(message) - 1 is the length of the message

    // some time all the bytes may not be written in one write() call this shall be handled in later labs
    if(bytes_written < 0)
    {
        perror("write");
        close(fd); // Close the file descriptor before exiting
        return 1;
    }

    // close the file descriptor to avoid FD leaks
    int fdc = close(fd);
    if(fdc < 0)
    {
        perror("close");
        return 1;
    }

    // Verify the file size using stat
    struct stat file_status;

    if(stat(file_path, &file_status) < 0)
    {
        perror("stat");
        return 1;
    }

    printf("File is created/updated : %s \n", file_path);

    print_mode_bits(file_status.st_mode);

    return 0;

}   

static void print_mode_bits(mode_t mode)
{
    printf("File mode bits: \n");
    printf("Owner: Read(%c) Write(%c) Execute(%c)\n",
           (mode & S_IRUSR) ? 'Y' : 'N',
           (mode & S_IWUSR) ? 'Y' : 'N',
           (mode & S_IXUSR) ? 'Y' : 'N');

    printf("Group: Read(%c) Write(%c) Execute(%c)\n",
           (mode & S_IRGRP) ? 'Y' : 'N',
           (mode & S_IWGRP) ? 'Y' : 'N',
           (mode & S_IXGRP) ? 'Y' : 'N');

    printf("Others: Read(%c) Write(%c) Execute(%c)\n",
           (mode & S_IROTH) ? 'Y' : 'N',
           (mode & S_IWOTH) ? 'Y' : 'N',
           (mode & S_IXOTH) ? 'Y' : 'N');

}
