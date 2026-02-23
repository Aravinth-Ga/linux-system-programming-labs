/*
    Lab 06: Understanding shared file offsets with dup()

    System calls used:
        - open()
        - dup()
        - write()
        - close()

    What this lab teaches:
        - How dup() creates file descriptors that share the same file offset.
        - How writes through different file descriptors advance the shared offset sequentially.
        - The difference between dup() and opening the same file twice.

    How to compile:
        1. Ensure you are in the directory containing this source file.
        2. Use the following command to compile:
            gcc -Wall -Wextra -O2 -g lab06_dup_shared_offset.c -o lab06
        3. Run the compiled program:
            ./lab06

    Hardware tested on:
        - Jetson Orin Nano

    Note:
        1. This program creates a file and writes to it through two file descriptors.
        2. The second descriptor is created using dup() from the first.
        3. Both writes occur sequentially in the file because they share the same offset.
        4. Output file will contain: "AAA\nBBB\n"
 */

 #include <fcntl.h>
 #include <stdio.h>
 #include <string.h>
 #include <unistd.h>
 #include <stdlib.h>


// Print error message and exit
static void die(const char* msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}


// Demonstrate how dup() creates a shared file offset
int main(int argc, char* argv[])
{
    const char* file_path = "dup_file_offset.txt";

    if(argc != 2 && argc != 0)
    {
        fprintf(stderr, "Usage : %s <file_path>\n", argv[0]);
        return 1;
    }

    if(argc != 0)
    {
        file_path = argv[1];
    }

    // Open file for writing
    int fd = open(file_path, O_WRONLY | O_APPEND | O_CREAT, 0644);

    if(fd < 0)
        die("open");

    // Write first line before duplicating the fd
    const char text[] = "Write 1: from original fd\n";
    size_t len = strlen(text);

    ssize_t bytes_written = write(fd, text, len);

    if(bytes_written < 0)
        die("write fd1");

    // Duplicate the fd - now fd and fd2 share the same file offset
    int fd2 = dup(fd);

    // Write second line using the duplicated fd
    const char text2[] = "Write 2: from fd2 (shares same offset)\n";
    size_t len2 = strlen(text2);
    
    ssize_t bytes_written_fd2 = write(fd2, text2, len2);

    if(bytes_written_fd2 < 0)
        die("write fd2");

    // Close both file descriptors
    if(close(fd) < 0)
        die("close fd1");

    if(close(fd2) < 0)
        die("close fd2");

    return 0;
}
