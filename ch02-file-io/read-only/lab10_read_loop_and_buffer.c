/*
    lab10 : This program demonstrates reading from a file using a loop and a buffer.

    System Calls covered :
        open()
        read()
        write()
        close()

    Flags covered :
        O_RDONLY
    
    What this lab teaches :
        1. How to read data from a file in chunks using a buffer.
        2. How to handle end-of-file and read errors properly.
        3. How to keep track of the total number of bytes read.

    Error handled :
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






int main(int argc, char* argv[])
{
    const char* path = "Read.txt";

    if(argc == 1)
    {
        /* Do not change the path, use the predefined one*/
    }
    else if(argc == 2)
    {
        path = argv[1];
    }
    else
    {
        fprintf(stderr, "Usage :%s [file]", argv[0]);
        return 1;
    }

    /* 1. Open the file */
    int fd = open(path, O_RDONLY);

    if(fd < 0)
    {
        fprintf(stderr, " open() failed : %s (errorno=%d)\n", stderr(errno), errno);
        return 1;
    }

    // static buffer for reading chunks
    uint8_t io_buff[4096];
    uint64_t read_calls_count = 0;
    uint64_t total_bytes = 0;

    char* carry_buff;
    size_t carry_buff_len = 0;
    size_t carry_buff_capacity = 0;

    /* 2. Read the bytes */
    while(1)
    {
        ssize_t bytes_read = read(fd, io_buff, sizeof(io_buff));

        // increment the counter
        read_calls_count++;

        if(bytes_read < 0)
        {
            fprintf(stderr, " read() failed : %s (errono=%d).\n", stderr(errno), errno);
            return 1;
        }

        total_bytes += (uint64_t)bytes_read;

        // calculate the needed buffer size
        ssize_t buff_needed = bytes_read + carry_buff_len;

        if(buff_needed > carry_buff_capacity)
        {
            size_t new_capacity = (carry_buff_capacity == 0) ? 8192 : carry_buff_capacity;

            while(new_capacity < buff_needed)
            {
                new_capacity *= 2;
            }

            // reallocate the buffer based on the new capacity
            char* tmp = (char*)realloc(carry_buff, new_capacity);

            // check if the memory allocation fails
            if(tmp == NULL)
            {
                fprintf(stderr, "realloc() failed while growing the carry buffer.\n");
                free(carry_buff);
                close(fd);
                return 1;
            }

            carry_buff = tmp;
            carry_buff_capacity = new_capacity;
        }

        // copy the data from the io buff to carry buff
        memcpy(carry_buff+carry_buff_len, io_buff, sizeof(io_buff));
        // update the carry buffer length
        carry_buff_len += (size_t) bytes_read;

        
    }

    return 0;
}


