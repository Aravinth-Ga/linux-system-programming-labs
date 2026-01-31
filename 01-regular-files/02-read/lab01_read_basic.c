/*
    lab01 : read a file basic demo.

    Systen call used :
        open()
        read()
        close()
    
    Flags used :
        O_RDONLY    (Open file in read-only mode)
    
    Error handled :
        perror() usage for error reporting.

    What this lab teaches :
        how to read data from a file using read() syscall.
        
*/

#include <fcntl.h>      // open(), O_RDONLY
#include <stdio.h>      // printf(), perror()
#include <unistd.h>     // close()
#include <stdint.h>     // __uint8_t


#define BUFFER_SIZE 16

/*  Main entry point of the program

    argc : Argument count
    argv : Argument vector (array of strings)

    Return : 0 on success and non-zero on error
*/
int main(int argc, char* argv[])
{
    const char* file_name = "basic_file_read.txt";

    if(argc == 1)
    {
        /* Do not change the file name, use the predefined one. */
    }
    else if(argc == 2)
    {
        file_name = argv[1];
    }
    else
    {
        // Invalid arguments
        fprintf(stderr, "Usage :%s [file]", argv[0]);
        return 1;
    }

    printf("Lab01 : read basic.\n");
    printf("Opening file in Read only mode.\n");

    //  Open the file in read-only mode
    int fd = open(file_name, O_RDONLY);

    if(fd<0)
    {
        // Error in open
        perror("open");
        return 1;
    }

    printf("The file open is success.\n");
    printf("Read a file now.\n");

    unsigned char buf[BUFFER_SIZE];
    while(1)
    {
        // Read data from the file
        ssize_t n = read(fd, buf, sizeof(buf));

        // EINTR is not handled in this example for simplicity
        if(n == -1)
        {
            // Error in read
            perror("read");
            close(fd);
            return 1;
        }

        // Check for end of file
        if(n == 0)
        {
            // EOF reached
            printf("\nEOF is reached.\n");
            break;
        }

        // Write the read data to standard output
        ssize_t written_bytes = write(STDOUT_FILENO, buf, n); // Write to standard output

        // Check for write error
        if(written_bytes == -1)
        {
            perror("write");
            close(fd);
            return 1;
        }

    }

    printf("Read operation is success.\n");
    printf("Now close the file.\n");

    // Close the file
    if(close(fd) < 0)
    {
        // Error in close
        perror("close");
        return 1;
    }

    printf("File is closed successfully.\n");
    printf("\n");

    return 0;
}
