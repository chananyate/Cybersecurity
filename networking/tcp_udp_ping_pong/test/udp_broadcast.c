#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "network_functions.h"

#define BUFFER_SIZE 1024

void udp_broadcast(const char *broadcast_ip, int port, const char *message) 
{
    struct sockaddr_in broadcast_addr = setup_address(broadcast_ip, port);

    int sockfd = create_udp_socket();

    enable_broadcast(sockfd);

    send_message(sockfd, &broadcast_addr, message, strlen(message));

    printf("Broadcast message sent: %s\n", message);

    close(sockfd);
}



int main(int argc, char *argv[]) 
{
    if (argc != 4) 
    {
        printf("Usage: %s [Broadcast IP] [Port] [Message]\n", argv[0]);
        return -1;
    }

    udp_broadcast(argv[1], atoi(argv[2]), argv[3]);

    return 0;

}