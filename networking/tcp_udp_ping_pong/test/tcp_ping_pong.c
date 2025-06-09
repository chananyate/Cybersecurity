#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "network_functions.h"
    
void tcp_server(int port) 
{
    int client_sock;
    struct sockaddr_in client_addr;
    char buffer[1024] = {0};

    int sockfd = create_tcp_socket();

    setup_binding(sockfd, port);

    start_listening(sockfd);

    client_sock = accept_connection(sockfd, &client_addr);

    printf("Client connected.\n");

    while (1) 
    {
        /*recv is a system call in C used for receiving data over a socket, it reads data from a socket and stores it into a buffer.
         'sizeof(buffer) - 1' ensures we leave space for the null terminator at the end of the string.
         '0' indicates the default flag for recv (no special options).*/
        ssize_t bytes_received = recv(client_sock, buffer, sizeof(buffer) - 1, 0);

        if (bytes_received > 0) 
        {
            buffer[bytes_received] = '\0'; /*Null terminate*/
            printf("Client: %s\n", buffer);

            printf("Server: ");
            fgets(buffer, sizeof(buffer), stdin);
            send(client_sock, buffer, strlen(buffer), 0);
        } 

        else 
        {
            printf("Client disconnected.\n");
            break;
        } 
    }

    close(client_sock);  /*Closes the specific client's socket*/
    close(sockfd);       /*Closes the listening socket that the server uses  to accept incoming cslls*/
}

void tcp_client(const char *server_ip, int port) 
{
    char buffer[1024] = {0};
    int sockfd = create_tcp_socket();
    ssize_t bytes_received;
    struct sockaddr_in server_addr = setup_address(server_ip, port);

    /*Attempts to establish a connection to the server specified by 'server_addr'.
     'sockfd' is the socket file descriptor, 'server_addr' contains the server's address and port information.
     'sizeof(server_addr)' specifies the size of the 'server_addr' structure.
      If the connection fails (returns a value < 0), the program exits with an error message.*/
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
        perror_exit("connect");
    
    while (1)
    {
        printf("Client: ");
        fgets(buffer, sizeof(buffer), stdin);
        send(sockfd, buffer, strlen(buffer), 0);

        bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);

        if (bytes_received > 0) 
        {
            buffer[bytes_received] = '\0'; 
            printf("Received: %s\n", buffer);
        } 

        else
        {
            printf("Server disconnected.\n");
            break;
        }  
    }
    close(sockfd);
}


int main(int argc, char *argv[]) 
{
    if (argc < 3 || argc > 4) 
    {
        printf("Usage: %s server <port> / client <server_ip> <port>\n", argv[0]);
        return -1;
    }

    if (strcmp(argv[1], "server") == 0) 
    {
        int port = atoi(argv[2]);
        tcp_server(port);
    } 

    else if (strcmp(argv[1], "client") == 0) 
    {
        const char *server_ip;
        int port;

        if (argc != 4) 
        {
            printf("Usage: %s client <server_ip> <port>\n", argv[0]);
            return -1;
        }

        server_ip = argv[2];
        port = atoi(argv[3]);
        tcp_client(server_ip, port);
    } 

    return 0;
}
