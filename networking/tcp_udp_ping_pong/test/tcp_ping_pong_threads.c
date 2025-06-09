#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "network_functions.h"

#define BUFFER_SIZE 1024

/*Function to handle the communication with a client*/
void *handle_client(void *client_sock_ptr)
{
    int client_sock = *(int *)client_sock_ptr; /*Retrieves the client socket descriptor*/
    char buffer[BUFFER_SIZE] = {0};

    printf("Client connected.\n");

    /*Starts a loop to continuously handle communication with the client*/
    while (1)
    {
        /*Receives data from the client*/
        ssize_t bytes_received = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0)
        {
            buffer[bytes_received] = '\0'; 
            printf("Received: %s\n", buffer);

            /*Responds only to the "Ping\n" message*/
            if (strcmp(buffer, "Ping\n") == 0)
            {
                send(client_sock, "Pong\n", strlen("Pong\n"), 0); /*Send response to client*/
                printf("Sent: Pong\n");
            }
            else
                printf("Ignoring unexpected message: %s\n", buffer);
        }

        else
        {
            printf("Client disconnected.\n");
            break; /*Exits loop when client disconnects or sends no data*/
        }
    }

    /*Closes the connection with the specific client*/
    close(client_sock);
    return NULL; /*End of the thread*/
}

void tcp_server(int port)
{
    int server_sock, client_sock;
    struct sockaddr_in client_addr;
    pthread_t thread_id; /*Thread ID for managing each client connection*/

    /*Creates the server socket*/
    server_sock = create_tcp_socket();
    setup_binding(server_sock, port);
    start_listening(server_sock);  /*Starts listening for client connections*/

    printf("Server listening on port %d...\n", port);

    /*Main loop to accept incoming client connections*/
    while (1)
    {
        /*Accept an incoming connection from a client*/
        client_sock = accept_connection(server_sock, &client_addr);

        /*Creates a new thread to handle communication with the client*/
        if (0 != pthread_create(&thread_id, NULL, handle_client, (void *)&client_sock))
        {
            perror("Failed to create thread");
            close(client_sock); /*Closes the client socket if thread creation fails*/
            continue; /*Continues accepting new connections*/
        }

        /*Detachs the thread so it will automatically clean up once done*/
        pthread_detach(thread_id);
    }

    /*Closes the listening socket when server exits*/
    close(server_sock);
}

void tcp_client(const char *server_ip, int port)
{
    char buffer[BUFFER_SIZE] = {0};
    int sockfd = create_tcp_socket();
    ssize_t bytes_received;
    struct sockaddr_in server_addr = setup_address(server_ip, port);

    /*Establishes a connection to the server*/
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        perror_exit("connect");

    /*Sends the "Ping\n" message to the server*/
    send(sockfd, "Ping\n", strlen("Ping\n"), 0);
    printf("Sent: Ping\n");

    /*Receives the response from the server*/
    bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);

    if (bytes_received > 0)
    {
        buffer[bytes_received] = '\0'; 
        printf("Received: %s\n", buffer);

        if (0 != strcmp(buffer, "Pong\n"))
            perror_exit("Unexpected message. Closing connection.\n");
    }
    else
        printf("Server disconnected.\n");

    /*Closes the client socket when done*/
    close(sockfd);
}

int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 4)
    {
        printf("Usage: %s server <port> / client <server_ip> <port>\n", argv[0]);
        return -1;
    }

    /*If running as a server*/
    if (0 == strcmp(argv[1], "server"))
    {
        int port = atoi(argv[2]);
        tcp_server(port); 
    }
    /*If running as a client*/
    else if (0 == strcmp(argv[1], "client"))
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
