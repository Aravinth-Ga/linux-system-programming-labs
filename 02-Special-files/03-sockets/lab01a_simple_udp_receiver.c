/*
    Lab 01a: Simple UDP Receiver (Testing Tool for Lab 01)

    System calls used:
        - socket()
        - bind()
        - recvfrom()
        - close()

    What this lab teaches:
        - How to create a UDP socket to receive incoming data.
        - How to bind a socket to a specific port using bind().
        - How to receive data from a network using recvfrom().
        - How to properly close a socket to avoid file descriptor leaks.


    How to compile:
        1. Make sure you are in the directory with this source file.
        2. Compile using:
            gcc -Wall -Wextra -O2 -g lab01a_simple_udp_receiver.c -o labre
        3. Run it:
             ./labre

    How to use this lab:
        1. This is a helper program to test lab01_udp_sender. Start this receiver first.
        2. It will print "Listening on port 8000..." and wait for incoming messages.
        3. In another terminal, run lab01_udp_sender (./lab01) to send a message.
        4. The receiver will display the received message from the sender.
        5. Both programs must use the same port (8000) and IP address to communicate.

    Hardware tested on:
        - Jetson Orin Nano

    Note : This is a simple testing tool for lab01. A more comprehensive UDP receiver
           will be implemented in lab02_udp_receiver with advanced features.
           Lab01a has the bare minimum code needed to validate lab01_udp_sender functionality.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>


static void die(const char* msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int main()
{
    // Step 1: Create a UDP socket to receive data
    int sock_fd_receive = socket(AF_INET, SOCK_DGRAM, 0);

    if(sock_fd_receive < 0)
        die("socket failed");

    // Step 2: Set up the address structure for binding to port 8000
    struct sockaddr_in addr_receive;

    memset(&addr_receive, 0x00, sizeof(addr_receive));

    addr_receive.sin_family = AF_INET;
    addr_receive.sin_port = htons(8000);
    addr_receive.sin_addr.s_addr = htonl(INADDR_ANY);

    // Step 3: Bind the socket to the local address and port
    bind(sock_fd_receive, (struct sockaddr*)&addr_receive, sizeof(addr_receive));

    printf("Listening on port 8000...\n");    

    // Step 4: Continuously wait and receive data from any sender
    char buff[256];
    uint32_t msg_count = 0;
    
    while(1)
    {
        recvfrom(sock_fd_receive, buff, sizeof(buff), 0, NULL, NULL);
        msg_count++;
        printf("Received [%u]: %s\n", msg_count, buff);
    }

    // Step 5: Close the socket to free the file descriptor (unreachable without break)
    if(close(sock_fd_receive) < 0)
        die("close");

    return 0;
}