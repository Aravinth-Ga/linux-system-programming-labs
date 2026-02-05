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
        O_APPEND
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
            return 1;
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
    int fd = open(fd, flag, mode);

    if(fd < 0)
    {
        perror("open");
        return 1;
    }

    // 4. Prepare the data buffer to the write

    char message[256];
    int pid = getpid();
    int n = snprintf(message, sizeof(message), 
                    "Lab05: Write with durability : pid = %d\n",
                    "This line should survive a crash only if synced.\n",
                    pid);

    // check for an error
    if(n < 0)
    {
        perror("snprintf is failed.\n");
    }
    else if(n >= sizeof(message))
    {
        printf("snprintf output was truncated because the formatted message did not fit in the buffer.\n");
    }

    // 5. Write the data


    return 0;
}