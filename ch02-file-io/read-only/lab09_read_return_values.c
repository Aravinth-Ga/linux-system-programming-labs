/*
    Lab 09: Handling return values of read()

    System Calls used:
        open()
        read()
        write()
        close()

    Flages used:
        O_RDONLY

    What this lab teach:
        1. How to handle return values of read() system call properly.
        2. Typical errors that can occur during read() operation.

    Error handled:
        1. Error in open()
        2. Error in read()
        3. Error in write()
        4. Error in close()
    
*/

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


#define BUFF_LENGTH 15


int main()
{
    const char* redable_file_path = "redable_file.txt";
    const char* empty_file_path = "empty_file.txt";

    /* 1. Read the normal file  */

    // open the file
    printf("Lab09. Read() return values.\n");
    printf("1. Read the normal file.\n");

    int fd = open(redable_file_path, O_RDONLY);

    if(fd < 0)
    {
        perror("open");
        return 1;
    }

    printf("    The file is opened successfully!\n");
    printf("    Reading the file now.\n");

    unsigned char buf_read[BUFF_LENGTH];
    size_t total = 0;
    while(1)
    {
        // read from the file
        ssize_t bytes_read = read(fd, buf_read, sizeof(buf_read));

        if(bytes_read < 0)
        {
            perror("read");
            close(fd);
            return 1;
        }

        // check for EOF
        if(bytes_read == 0)
        {
            printf("    EOF is reached for the file : %s \n", redable_file_path);
            break;
        }
        
        // write to standard output
        ssize_t n_read = bytes_read;
        while(1){
            ssize_t written_bytes = write(STDOUT_FILENO, buf_read, n_read);
            if(written_bytes < 0)
            {
                if(errno == EINTR){
                    // Interrupted by signal, retry writing
                    continue;
                } 
                else
                {
                    perror("write");
                    close(fd);
                    return 1;
                }
            }
            if(written_bytes < n_read)
            {
                // Partial write, adjust buffer and continue writing
                memmove(buf_read, buf_read + written_bytes, n_read - written_bytes);
                n_read -= written_bytes;
            } 
            else 
            {
                // All bytes written
                break;
            }
        }      

        // update total bytes read
        total += bytes_read;
    }

    printf("Total No of Bytes read is : %zd\n", total);

    printf("    The file read is success for the redable_file.txt \n");
    printf("    Now close the file.\n");
    if(close(fd) < 0)
    {
        perror("close");
        return 1;
    }
    printf("    The file is closed.\n");


    /* 2. Read the Empty file  */

    printf("2. Read the Empty file.\n");

    int fd2 = open(empty_file_path, O_RDONLY);

    if(fd2 < 0)
    {
        perror("open");
        return 1;
    }

    // initalise the variables to 0
    total = 0;

    // Read the file
    while(1)
    {
        // read from the file
        ssize_t bytes_read = read(fd2, buf_read, sizeof(buf_read));

        printf("    The number of bytes read in this iteration : %zd\n", bytes_read);

        if(bytes_read < 0)
        {
            perror("read");
            close(fd2);
            return 1;
        }

        if(bytes_read == 0)
        {
            printf("    EOF is reached.\n");
            break;
        }

        total += bytes_read;

    }

    // close the file
    if(close(fd2) < 0)
    {
        perror("close");
        return 1;
    }

    /* 3. Read the invalid file  */

    printf("3. Try to read the file which is already close.\n");
    ssize_t bytes_read = read(fd2, buf_read, sizeof(buf_read));

    // check for read error as the file is already closed
    if(bytes_read == -1 && errno == EBADF)
    {
        perror("read");
        return 1;
    }

    return 0;
}