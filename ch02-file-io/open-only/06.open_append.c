/*
    lab 06: Open a file in append mode

    System calls used:
        open()
        write()
        close()
    
    Flags used:
        O_APPEND    (Open file in append mode)
        O_WRONLY    (Open file in write-only mode)
    
    Mode (permission bits) used (only when creating a new file):
        0644 : owner can read/write, group can read, others can read.

    What this lab teaches:
        - O_APPEND opens the file in append mode. All write operations will
          append data to the end of the file, regardless of the current file
          offset.
        - umask effects the permission bits of the newly created file.

*/


#include <fcntl.h>    // For open(), O_APPEND, O_WRONLY, O_CREAT
#include <stdio.h>    // For printf() and perror()
#include <unistd.h>   // For close()
#include <sys/stat.h> // For mode constants




int main(int argc, char* argv[])
{
    const char* file_name = "lab06_logfile_01.log";

    if(argc == 1)
    {
        /* Do not change the file path, use the predefined one */
    }
    else if(argc == 2)
    {
        file_name = argv[1];
    }
    else
    {
        fprintf(stderr, "Usage : %s [file]\n", file_name);
        return 1;
    }

    // mode (permission bits) for the new file if created
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH ;

    // flags for open() call
    int flag = O_WRONLY | O_CREAT | O_APPEND;

    printf("Lab06 : open a file in append mode.\n");
    printf("calling open(filepath, flags, mode)\n");
    // open the file in append mode
    int fd = open(file_name, flag, mode);

    if(fd < 0)
    {
        perror("open");
        return 1;
    }

    printf("The log file is creation is success.\n");
    printf("Write the log file some data");

    // write the file
    const char msg[] = "The log file is generated and written successfully!";
    static ssize_t bytes_written;
    
    do
    {
        bytes_written = write(fd, msg, sizeof(msg));

        if(bytes_written < 0)
        {
            perror("write");
            return 1;
        }

    }while(bytes_written < (ssize_t)sizeof(msg));

    printf("Now all the bytes are written into the log file.\n");
    printf("Close the log file now.\n");

    // close the file to avoid the fd leaks
    if(close(fd) < 0)
    {
        perror("close");
        return 1;
    }

    printf("[Ok] The lab06 is completed. The file is opended, updated and closed successfully");

    return 0;

}
