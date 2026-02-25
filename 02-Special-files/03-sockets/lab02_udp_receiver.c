/*

    Lab 02: Basic UDP Socket Receiver

    System calls used:
        - socket()
        - bind()
        - recvfrom()
        - close()

    What this lab teaches:
        - Creating a UDP socket for network communication
        - Binding a socket to listen on all network interfaces (INADDR_ANY)
        - Receiving incoming messages with recvfrom()
        - Proper socket cleanup to prevent resource leaks


    How to compile:
        1. Ensure you are in the directory containing this source file.
        2. Use the following command to compile:
            gcc -Wall -Wextra -O2 -g lab02_udp_receiver.c -o lab02
        3. Run the compiled program:
             ./lab02

    How to validate/test this lab:
        1. First, start this receiver program (lab02) to bind to port 8000 and wait for incoming data.
           The receiver will block on recvfrom() until a message arrives.
        2. Once the receiver is running and listening, execute the UDP sender program (lab01) in another terminal.
        3. If everything works correctly, you should see the transmitted message displayed on the receiver side.

    Hardware tested on:
        - Jetson Orin Nano

    Note : This is a basic UDP receiver that demonstrates connectionless socket communication.
           The receiver listens on all network interfaces (INADDR_ANY) and processes incoming messages.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8000

int main()
{
    char recv_buffer[1024];

    // 1. Create a socket
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if(sock_fd < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Configure the server address
    struct sockaddr_in server_addr;
    
    memset(&server_addr, 0x00, sizeof(server_addr));
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8000);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // 3. Bind the socket
    if(bind(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("UDP receiver listening on port %d....\n", PORT);

    // 4. Receive the data from the client
    
    while(1)
    {
        int bytes_received = recvfrom(sock_fd, recv_buffer, sizeof(recv_buffer), 0,NULL, NULL);

        if(bytes_received < 0)
        {
            perror("recvfrom failed");
            break;
        }

        recv_buffer[bytes_received] = '\0'; // NULL terminate receive data

        printf("Received Message : %s \n", recv_buffer);

    }

    if(close(sock_fd) < 0)
    {
        perror("close");
        exit(EXIT_FAILURE);
    }

    return 0;
}