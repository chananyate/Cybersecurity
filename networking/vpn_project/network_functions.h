#ifndef NETWORK_FUNCTIONS_H
#define NETWORK_FUNCTIONS_H

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int create_udp_socket();

int create_tcp_socket();

struct sockaddr_in setup_address(const char *ip, int port);

void setup_binding(int sockfd, int port);

void start_listening(int sockfd);

int accept_connection(int sockfd, struct sockaddr_in *client_addr);

void enable_broadcast(int sockfd);

void send_message(int sockfd, struct sockaddr_in *addr, const char *message, size_t length);

void receive_message(int sockfd, char *buffer, size_t length, struct sockaddr_in *addr);

void connect_to_server(int sockfd, struct sockaddr_in *server_addr);

void perror_exit(const char *msg);

int create_tun_interface();

void configure_network_interface();

void setup_routing_rules();

void cleanup_routing_rules();

int setup_udp_socket(struct sockaddr *server_addr, socklen_t* addr_len);

void setup_signal_handlers();


#endif
