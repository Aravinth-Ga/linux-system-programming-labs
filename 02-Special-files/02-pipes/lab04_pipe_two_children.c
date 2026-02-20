/*
    Lab 04: Pipe with two children writing and parent reading

    System calls used:
        - pipe()
        - fork()
        - read()
        - write()
        - close()
        - wait()/waitpid()

    What this lab teaches:
        - How to create multiple child processes using fork().
        - How two child processes can write to pipes independently.
        - How the parent process reads data from children.
        - Proper file descriptor handling in each process path.

    How to compile:
        1. Ensure you are in the directory containing this source file.
        2. Use the following command to compile:
            gcc -Wall -Wextra -O2 -g lab04_pipe_two_children.c -o lab04
        3. Run the compiled program:
            ./lab04

    Hardware tested on:
        - Jetson Orin Nano

    Note:
        1. Partial read/write handling may be minimal in this basic lab.
        2. Child-write/parent-read flow is a design choice for IPC learning.
 */


  #include <fcntl.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <sys/wait.h>
 #include <errno.h>
 #include <string.h>


/*
 * die - Print an error message and terminate the process
 * @msg: Context string passed to perror()
 *
 * Used for unrecoverable syscall failures in this lab.
 */
 static void die(const char* msg)
 {
    perror(msg);
    exit(EXIT_FAILURE);
 }

static void read_and_print(int read_fd, const char *text)
{
    char buf[256];
    ssize_t r = read(read_fd, buf, sizeof(buf) - 1);
    if (r < 0) {
        die("parent read");
    }
    buf[r] = '\0';
    printf("%s received (%zd bytes): %s", text, r, buf);
}


 int main()
 {
    //process 1 & 2
    int p1[2];
    int p2[2];

    // create a pipe
    if(pipe(p1) < 0)
        die("pipe p1");

    if(pipe(p2) < 0)
        die("pipe p2");        

    // create a copy
    pid_t c1 = fork();

    if(c1 < 0)
        die("fork c1");
    
    if(c1 == 0)
    {
        // close the unused port
        if(close(p1[0]) < 0)
            die("close p1[0]");
        if(close(p2[0]) < 0)
            die("close p2[0]");
        if(close(p2[1]) < 0)
            die("close p2[1]");        
            
        const char* msg1 = "Hi parent! This is message from Child 1.\n";
        size_t len1 = strlen(msg1);

        ssize_t bytes_written1 = write(p1[1], msg1, len1);

        if(bytes_written1 < 0)
            die("write p1[1]");

        if((size_t)bytes_written1 < len1)
            fprintf(stderr, "Partial Write P1[1]. Wrote %zd of %zu.\n", bytes_written1, len1);

        if(close(p1[1]) < 0)
            die("close p1[1]");

        exit(EXIT_SUCCESS);
    }

    pid_t c2 = fork();

    if(c2 < 0)
        die("fork c1");
    
    if(c2 == 0)
    {
        // close the unused port
        if(close(p2[0]) < 0)
            die("close p2[0]");
        if(close(p1[0]) < 0)
            die("close p1[0]");
        if(close(p1[1]) < 0)
            die("close p1[1]");        
            
        const char* msg1 = "Hi parent! This is message from Child 2.\n";
        size_t len1 = strlen(msg1);

        ssize_t bytes_written = write(p2[1], msg1, len1);

        if(bytes_written < 0)
            die("write p2[1]");

        if((size_t)bytes_written < len1)
            fprintf(stderr, "Partial Write P2[1]. Wrote %zd of %zu.\n", bytes_written, len1);

        if(close(p2[1]) < 0)
            die("close p2[1]");

        exit(EXIT_SUCCESS);
    }    

    // Parent only reads, so close both write ends
    if (close(p1[1]) < 0) 
        die("parent close p1[1]");

    if (close(p2[1]) < 0) 
        die("parent close p2[1]");

    // Read from both children
    read_and_print(p1[0], "parent");
    read_and_print(p2[0], "parent");

    if (close(p1[0]) < 0) 
        die("parent close p1[0]");

    if (close(p2[0]) < 0) 
        die("parent close p2[0]");

    // Reap both children
    int st1 = 0, st2 = 0;
    if (waitpid(c1, &st1, 0) < 0)
        die("waitpid c1");

    if (waitpid(c2, &st2, 0) < 0)
        die("waitpid c2");

    if (WIFEXITED(st1)) 
        printf("child1 exit=%d \n", WEXITSTATUS(st1));

    if (WIFEXITED(st2)) 
        printf("child2 exit=%d \n", WEXITSTATUS(st2));

    return 0;    
    
 }