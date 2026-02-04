/*
    lab 07: Open a file in append mode

    System calls used:
        open()
        write()
        close()
        lseek()
    
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
        - lseek() manually changes the file offset.
        - O_APPEND is a kernel offset mode, so even if you change the file offset
          using lseek(), the write operation will still append data to the end
          of the file.

*/


#include <fcntl.h>    // For open(), O_APPEND, O_WRONLY, O_CREAT
#include <stdio.h>    // For printf() and perror()
#include <unistd.h>   // For close()
#include <sys/stat.h> // For mode constants
#include <string.h>   // For strcmp()

/* Main entry point of the program.

    argc : Argument count.
    argv : Argument vector (array of strings).

    Return value : 0 on success, non-zero on failure.
 */
int main(int argc, char* argv[])
{
    const char* file_name = "lab07_logfile_01.log";
    int use_append_mode = 0;

    if(argc == 1)
    {
        /* Do not change the file path, use the predefined one */
    }
    else if(argc == 2)
    {
        if(strcmp(argv[1], "append") == 0)
        {
            // set the append mode as append
            use_append_mode = 1;
        }
        else if(strcmp(argv[1], "lseek") == 0)
        {
            use_append_mode = 0;
        }
        else
        {
            fprintf(stderr, "Usage: %s [append | lseek][file]\n",argv[0]);
            return 1;
        }
    }
    else if(argc == 3)
    {
        if(strcmp(argv[1], "append") == 0)
        {
            use_append_mode = 1;
        }
        else if(strcmp(argv[1],"lseek") == 0)
        {
            use_append_mode = 0;
        }
        else
        {
            fprintf(stderr, "Usage : %s [append | lseek] [file]\n", argv[0]);
            return 1;
        }

        file_name = argv[2];
    }
    else
    {
        fprintf(stderr, "Usage : %s [append | lseek] [file]\n", argv[0]);
        return 1;
    }

    // mode (permission bits) for the new file if created
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH ;

    // flags for open() call
    int flag = O_WRONLY | O_CREAT;

    if(use_append_mode == 0x01)
    {
        flag |= O_APPEND;
        printf("Lab07 : open a file in append mode.\n");
    }
    else
    {
        printf("Lab07 : open a file in lseek mode.\n");
    }

    printf("calling open(filepath, flags, mode)\n");
    // open the file in append mode
    int fd = open(file_name, flag, mode);

    if(fd < 0)
    {
        perror("open");
        return 1;
    }

    printf("The log file is creation is success.\n");
    printf("Write the log file some data.\n");

    // write the file
    const char msg[] = "The log file is generated and written successfully!\n";
    ssize_t bytes_written = 0;
    ssize_t total = 0;
    off_t current_offset;

    // if not in append mode, move the file offset to the end of the file
    if(use_append_mode == 0x00)
    {
        // move the file offset to the end of the file
        current_offset = lseek(fd, 0, SEEK_END);

        // check for lseek error
        if(current_offset < 0)
        {
            perror("lseek");
            close(fd);
            return 1;
        }
    }

    // write the complete message into the file
    while(bytes_written < (ssize_t)(sizeof(msg)-1))
    {
        // write the message to the file
        bytes_written = write(fd, msg + total, sizeof(msg) - total - 1);

        // check for write error
        if(bytes_written < 0)
        {
            perror("write");
            close(fd);
            return 1;
        }
        
        // update the total bytes written
        total += bytes_written;
    }

    // get the current file offset after write
    current_offset = lseek(fd, 0, SEEK_CUR);

    // check for lseek error
    if(current_offset < 0)
    {
        perror("lseek");
        close(fd);
        return 1;
    }

    printf("Current offset after write: %ld\n", (long)current_offset);

    printf("Now all the bytes are written into the log file.\n");
    printf("Close the log file now.\n");

    // close the file to avoid the fd leaks
    if(close(fd) < 0)
    {
        perror("close");
        return 1;
    }

    printf("[Ok] The lab07 is completed. The file is opened, updated and closed successfully.\n");

    return 0;

}
