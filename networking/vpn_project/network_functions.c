#include "network_functions.h"

int create_udp_socket() 
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0) 
        perror_exit("socket");

    return sockfd;
}

int create_tcp_socket() 
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) 
        perror_exit("socket");

    return sockfd;
}

/*Function to set up a sockaddr_in structure with a given IP and port.
  Can be used later for socket operations like binding or connecting.
  This struct ensures that the address is correctly formatted in a structure suitable for network communication.*/
struct sockaddr_in setup_address(const char *ip, int port) 
{
    struct sockaddr_in addr;  /*Declares the sockaddr_in structure to store the address information (ip and port)*/

    memset(&addr, 0, sizeof(addr));  /*Initializes the structure to zero to avoid any undefined values*/

    addr.sin_family = AF_INET;  /*Sets the address family to AF_INET, which indicates IPv4*/

    /*Converts the port number to network byte order and stores it in addr.sin_port.
      htons() ensures the port is in the correct format (big-endian) for network transmission.*/
    addr.sin_port = htons(port);

    /*Converts the provided IP address from string format to binary format.
      inet_pton() stores the result in addr.sin_addr in binary form (struct in_addr)*/
    inet_pton(AF_INET, ip, &addr.sin_addr);

    return addr;
}

/*Function to bind a socket to a specified port on the local machine*/
void setup_binding(int sockfd, int port) 
{
    /*Sets up the address structure using the specified port and binds it to all available network interfaces ("0.0.0.0")*/
    struct sockaddr_in addr = setup_address("0.0.0.0", port);

    /*Attempts to bind the socket to the specified address and port.
      sockfd: The socket file descriptor to bind.
      (struct sockaddr *)&addr: The address structure that contains the port and IP address to bind the socket to.
      sizeof(addr): The size of the address structure.
      If binding fails, call perror_exit() to print an error message and exit the program.*/
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) 
        perror_exit("bind");
}

void start_listening(int sockfd) 
{
    if (listen(sockfd, 10) < 0)  /*10 is the requests backlog. It permits 10 requests to connect before it starts rejecting them*/
        perror_exit("listen");
}

int accept_connection(int sockfd, struct sockaddr_in *client_addr) 
{

    socklen_t addr_len = sizeof(*client_addr);

    int client_sock = accept(sockfd, (struct sockaddr *)client_addr, &addr_len);

    if (client_sock < 0) 
        perror_exit("accept");

    return client_sock;
}

void enable_broadcast(int sockfd) 
{
    int broadcast = 1;  /*Enables the broadcast option*/

    /*setsockopt is asystem call used to configure options for a socket. 
      SOL_SOCKET tells setsockopt that we're configuring a socket level option. 
      SO_BROADCAST is the socket option that enables broadcasting*/
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
        perror_exit("setsockopt");
}

void send_message(int sockfd, struct sockaddr_in *addr, const char *message, size_t length) 
{
    sendto(sockfd, message, length, 0, (struct sockaddr *)addr, sizeof(*addr));
}

/*Function to receive a message from a socket and store the sender's address*/
void receive_message(int sockfd, char *buffer, size_t length, struct sockaddr_in *addr) 
{   
    socklen_t addr_len = sizeof(*addr);  /*Defines the size of the address structure to be used in recvfrom*/

    /*Receives the message using the recvfrom function.
      sockfd: The socket file descriptor to receive the message from.
      buffer: The buffer to store the received message.
      length: The maximum number of bytes to read into the buffer.
      0: Flags (0 means no special flags are used).
      addr: The sockaddr_in structure that will be populated with the sender's address.
      addr_len: The size of the address structure, which will be updated by recvfrom with the actual address length.*/
    recvfrom(sockfd, buffer, length, 0, (struct sockaddr *)addr, &addr_len);
}

void connect_to_server(int sockfd, struct sockaddr_in *server_addr)
{
    if (connect(sockfd, (struct sockaddr *)server_addr, sizeof(*server_addr)) < 0)
        perror_exit("connect");
}

void perror_exit(const char *msg) 
{
    perror(msg);
    exit(EXIT_FAILURE);
}