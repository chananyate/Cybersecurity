#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "network_functions.h"

#define PORT 8000
#define BUFFER_SIZE 1024

int server_sock;

volatile sig_atomic_t shutdown_flag = 0;

void signal_handler(int signum)
{
	printf("\nSIGINT (ctrl c) signal received. Initiating shut down...");
	shutdown_flag = 1;
}

void handle_client (int client_sock)
{
	char buffer[BUFFER_SIZE];
	const char *response;

	/*Used to read data from the socket and write it into the buffer.
	  Returns the number of bytes read from the socket.*/
	ssize_t bytes_read = recv(client_sock, buffer, sizeof(buffer) -1, 0);

	if (bytes_read < 0)
	{
		perror("recv");
		close(client_sock);
		return;
	}  

	buffer[bytes_read] = '\0';

	/*Checks for an HTTP GET request and sends a response back to the client*/
	if (0 == strncmp(buffer, "GET", 3))
	{
		/*Defines a a simple HTTP response as a string which will be sent back to the client
		  It is composed of a status line indicating that the request was successful (200), 
		  a header that specifies the content (html), and then the message itself.*/
		response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\nHello World!";
		send(client_sock, response, strlen(response), 0);
	}
	close(client_sock);
}

int main() 
{
	int client_sock;

	struct sockaddr_in client_addr;

	signal(SIGINT, signal_handler);

	server_sock = create_tcp_socket();

	setup_binding(server_sock, PORT);

	start_listening(server_sock);

	printf("Listening on port %d...\n", PORT);

	while(!shutdown_flag)
	{
		client_sock = accept_connection(server_sock, &client_addr);

		printf("Client connected.\n");

		if (shutdown_flag)
			break;

		handle_client(client_sock);
	}

	printf("Shutting down server...");
	
	close(server_sock);

	return 0;
}