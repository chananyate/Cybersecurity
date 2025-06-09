#include <stdio.h>      /*printf*/
#include <stdlib.h>     /*atoi*/
#include <string.h>     /*strcpy*/
#include <arpa/inet.h>  /*inet_pton, struct sockaddr_in*/
#include <unistd.h>     /*close()*/
#include "network_functions.h"

#define BUFFER_SIZE 1024

/*Client side: Sends a "Ping" message to the specified IP and port using UDP.
  Waits for a response from the server and prints it.*/
void udp_ping(const char *ip, int port) 
{
	int sockfd = create_udp_socket();
    /*Sets up the server's address in binary/network form*/
    struct sockaddr_in server_addr = setup_address(ip, port);

    char buffer[BUFFER_SIZE] = {0};

    strcpy(buffer, "Ping");

    printf("Sending: %s\n", buffer);
    /*Sends the "Ping" message to the server*/
    send_message(sockfd, &server_addr, buffer, strlen(buffer));
    /*Waits for a response from the server*/
    receive_message(sockfd, buffer, BUFFER_SIZE, NULL);

    printf("Received: %s\n", buffer);

    close(sockfd);
}

/*Server side: Waits for a "Ping" message from a client and responds with "Pong".
  This runs in a loop to handle multiple messages.*/
void udp_pong(int port) 
{
    int sockfd = create_udp_socket();
    /*Declares an empty struct to stores client's address*/
    struct sockaddr_in client_addr;

    char buffer[BUFFER_SIZE] = {0};

    /*Sets up the socket to bind to the specified port*/
    setup_binding(sockfd, port);

    while (1) 
    {  
        /*Receives a message from the client and stores it in the client_addr struct*/
        receive_message(sockfd, buffer, BUFFER_SIZE, &client_addr);

        printf("Received: %s\n", buffer);

        strcpy(buffer, "Pong");
        /*Sends the response back to the client*/
        send_message(sockfd, &client_addr, buffer, strlen(buffer));

        printf("Sent: %s\n", buffer);
    }

    close(sockfd);
}

 /*The starting point of the program. Decides whether to run `ping` (client) or
  `pong` (server) function based on the command line arguments.
   Usage: For ping (client): ./udp_ping_pong ping <IP> <Port>
   For pong (server): ./udp_ping_pong pong <Port>*/
int main(int argc, char *argv[]) 
{
    /*Determines the mode of operation*/
    if (argc < 2) 
    {
        printf("Usage: %s [ping/pong] [IP] [Port]\n", argv[0]);
        return -1;
    }

    /*Runs in client mode (ping)*/
    if (strcmp(argv[1], "ping") == 0 && argc == 4) 
        udp_ping(argv[2], atoi(argv[3]));

    /*Runs in server mode (pong)*/
    else if (strcmp(argv[1], "pong") == 0 && argc == 3) 
        udp_pong(atoi(argv[2]));
    else 
    {
        printf("Invalid arguments.\n");
        return -1;
    }

    return 0;
}