# Sockets

Small exercises around UDP sockets. Labs cover creating sockets,
sending and receiving datagrams.

- `lab01_udp_sender.c`
- `lab01a_simple_udp_receiver.c`
- `lab02_udp_receiver.c`

## Notes: What I Learned About Sockets (UDP)

### The Basics
A socket is just a file descriptor for network communication. UDP lets us send messages to a specific address without connecting first.

### Creating a Socket
```c
int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
```
- AF_INET = IPv4 networks
- SOCK_DGRAM = UDP (sends complete messages)
- Returns file descriptor on success, -1 on error

### The Address Structure
```c
struct sockaddr_in addr;
addr.sin_family = AF_INET;
addr.sin_port = htons(8000);              // port number, must use htons()
addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // IP address
```
- **htons()** converts port from normal number to network byte order
- **inet_addr()** converts IP string to binary format

### Sending (Sender Side)
```c
sendto(sock_fd, message, strlen(message), 0, (struct sockaddr *)&addr, sizeof(addr));
```
- Sends message to the address we specified
- No connection needed - just send to the address
- Returns bytes sent on success, -1 on error

### Receiving (Receiver Side)
```c
bind(sock_fd, (struct sockaddr*)&addr, sizeof(addr));  // bind to port
recvfrom(sock_fd, buffer, sizeof(buffer), 0, NULL, NULL);  // wait for message
```
- `bind()` - attach socket to a port so we can receive on it
- `recvfrom()` - wait for a message from anyone
- Use `INADDR_ANY` in bind to listen on all interfaces

### Important Things
- Always close socket when done: `close(sock_fd)`
- Check errors: socket/bind/recvfrom return -1 on error, then `perror()`
- Each `recvfrom()` gets one complete message (not a stream like files)
- UDP is one-way per message - no connection stays open
- Messages can be lost or arrive out of order







