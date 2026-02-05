/*
    lab05 : write durability with fsync and fdatasync

    system calls used:
        open()
        write()
        fsync()
        fdatasync()
        close()

    flags used:
        O_WRONLY
        O_CREAT
        O_TRUNC
    
    error handling:
        perror() and stderr messages for failures/partial writes.

    what this labe teaches:
        - write() puts data in page cache, it may not be durable immediately.
        - fsync() flushes file data and metadata to stable storage.
        - fdatasync() flushes file data and only essential metadata.
        - A robust “write + sync + close” pattern.
        - Optional safer pattern: write to temp file, fsync, then rename (atomic replace).

*/

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>


typedef enum
{
    MODE_NONE = 0,
    MODE_FSYNC,
    MODE_FDATASYNC
}mode_type;

static void helper_func(const char* argv)
{
    // Stream the helper info on the stdout screen
    fprintf(stdout, "Usage : %s [--none | --fsync | --fdatasync] [file]\n"
                    "Default : --fsync \n"
                    "Example :\n"
                    "  %s  --fsync  sample.txt\n"
                    "  %s  --fdatasync sample.txt\n"
                    "  %s  --none sample.txt\n", argv, argv, argv, argv);
}

static int write_func(int fd, const void* buff, size_t len)
{
    const char* data = (const char*) buff;
    size_t total_bytes = 0;
    int retry_count = 0;

    while(total_bytes < len)
    {
        ssize_t bytes_written = write(fd, data + total_bytes, len - total_bytes);

        if(bytes_written < 0)
        {
            if(errno == EINTR)
                continue;
            else
                return -1;

        }
        else if(bytes_written == 0)
        {
            errno = EIO;
            return -1;
        }
        else
        {
            total_bytes += bytes_written;
        }
    }

    return 0;
}

int main(int argc, char* argv[])
{
    const char* file_path = "lab05_write_fsync_fdatasync.txt";
    
    // 1. Define the default durability mode
    mode_type durability_mode = MODE_FSYNC;

    // 2. Verify the command line argument and update the file path and durability mode
    for(int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "--none") == 0)
        {
            durability_mode = MODE_NONE;
        }
        else if(strcmp(argv[i], "--fsync") == 0)
        {
            durability_mode = MODE_FSYNC;
        }
        else if(strcmp(argv[i], "--fdatasync") == 0)
        {
            durability_mode = MODE_FDATASYNC;
        }
        else if((strcmp(argv[i], "--help") == 0) || (strcmp(argv[i], "--h") == 0))
        {
            helper_func(argv[0]);
            return 0;
        }
        else
        {
            file_path = argv[i];
        }
    }

    // 3. Configure the Open flags, permission mode and open the file

    // Configure open flags
    int flag = O_WRONLY | O_CREAT | O_TRUNC;

    // Configure the Permission mode (0644)
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    // Open the file
    int fd = open(file_path, flag, mode);

    if(fd < 0)
    {
        perror("open");
        return 1;
    }

    // 4. Prepare the data buffer to the write

    char message[256];
    int pid = getpid();
    int no_of_bytes = snprintf(message, sizeof(message), 
                    "Lab05: Write with durability : pid = %d\n"
                    "This line should survive a crash only if synced.\n",
                    pid);

    // check for an error
    if(no_of_bytes < 0)
    {
        perror("snprintf");
        close(fd);
        return 1;
    }
    else if((size_t)no_of_bytes >= sizeof(message))
    {
        fprintf(stderr,"snprintf output was truncated because the formatted message did not fit in the buffer.\n");
        close(fd);
        return 1;
    }

    // 5. Write the data
    
    if(write_func(fd, message, (size_t)no_of_bytes) < 0)
    {
        perror("write");
        close(fd);
        return 1;
    }

    // 6. Flush the data into the disk

    if(durability_mode == MODE_FSYNC)
    {
        if(fsync(fd) < 0)
        {
            perror("fsync");
            close(fd);
            return 1;
        }

        printf("Durability mode is fsync(). data + metadata is flushed.\n");
    }
    else if(durability_mode == MODE_FDATASYNC)
    {
        if(fdatasync(fd) < 0)
        {
            perror("fdatasync");
            close(fd);
            return 1;
        }
        printf("Durability mode is fdatasync(). Data + minimal data to retrive the file is flushed.\n");
    }
    else
    {
        printf("Durability mode is none. write() only. data may still in the page cache.\n");
    }

    // 7. Close the file
    if(close(fd) < 0)
    {
        perror("close");
        return 1;
    }

    return 0;
}