/*

    Lab 01: Basic UDP Socket Sender

    System calls used:
        - socket()
        - sendto()
        - close()

    What this lab teaches:
        - socket() creates an endpoint for network communication.
        - AF_INET specifies IPv4 address family, SOCK_DGRAM specifies UDP (datagram) protocol.
        - struct sockaddr_in is used to specify destination address for UDP transmission.
        - inet_addr() converts a dotted-decimal IPv4 address string to binary format.
        - htons() converts the port number from host byte order to network byte order.
        - sendto() sends data to a specified destination address (connectionless).
        - It is important to close socket descriptors to prevent file descriptor leaks.


    How to compile:
        1. Ensure you are in the directory containing this source file.
        2. Use the following command to compile:
            gcc -Wall -Wextra -O2 -g lab01_udp_sender.c -o lab01
        3. Run the compiled program:
             ./lab01

    How to validate/test this lab:
        1. First, start the UDP receiver (lab01a_simple_udp_receiver) on the target host (192.168.0.1:8000).
           This receiver must be listening and waiting for incoming data before the sender transmits.
        2. Once the receiver is running and listening, execute this sender program (lab01).
        3. If everything works correctly, you should see the transmitted message displayed on the receiver side.

    Hardware tested on:
        - Jetson Orin Nano

    Note : This is a basic UDP sender that demonstrates connectionless socket communication.
           The receiver must be running on the target host to receive the message.
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

static void die(const char* msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int main()
{
    /* Step 1: Create a UDP socket
       - AF_INET: IPv4 address family
       - SOCK_DGRAM: UDP datagram socket (connectionless)
       - 0: Default protocol for the specified socket type */
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if(sock_fd < 0)
        die("socket failed");

    // Step 2: Initialize and configure the destination address structure
    struct sockaddr_in addr;

    memset(&addr, 0x00, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(8000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Step 3: Prepare the datagram message to be sent
    const char *message = "Hi! This is a test message from UDP sender.";
    static uint32_t count = 0;
    
    // Step 4: Send the message to the configured destination address
    while(count < 65535)
    {
        ssize_t ret = sendto(sock_fd, message, strlen(message), 0, (struct sockaddr *)&addr, sizeof(addr));
        
        if(ret < 0)
            die("sendto failed");
    
        printf("Message sent %u times...\n", count + 1);
        count++;
        
        // Add delay between sends so receiver can process each message
        usleep(1000);
    }


    /* Step 5: Close the socket fd to prevent fd leaks */
    if(close(sock_fd) < 0)
        die("close");

    return 0;
}