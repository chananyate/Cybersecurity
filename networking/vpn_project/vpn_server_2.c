#include <stdio.h>      /*printf*/
#include <stdlib.h>     /*system()*/
#include <string.h>     /*memset*/
#include <unistd.h>     /*read(), write()*/
#include <assert.h>     
#include <sys/socket.h> /*socket()*/
#include <sys/types.h>  /*system data types used by other headers*/
#include <sys/stat.h>   /*file status*/
#include <sys/ioctl.h>  /*I/O control operations - needed for network interface configuration*/
#include <netdb.h>      /*network database operations - provides getaddrinfo()*/
#include <fcntl.h>      /*file control - open(), fcntl()*/
#include <signal.h>     /*signal handling - for cleanup on program termination*/
#include <linux/if.h>   /*linux network interface definitions*/
#include <linux/if_tun.h> /*linux tun device definitions*/

#define SERVER_PORT 7000    /*udp port number for VPN traffic*/
#define MAX_PACKET_SIZE 1400
#define SERVER_BIND_ADDRESS "0.0.0.0"   /*server bind address - allows connections from any ip*/
#define VPN_NETWORK "10.8.0.0/16"
#define SERVER_VPN_IP "10.8.0.1"

int get_max(int first_num, int second_num) 
{
    if (first_num > second_num) 
        return first_num;
    else 
        return second_num;
}

int create_tun_interface() 
{
    struct ifreq interface_request;
    int tun_file_descriptor, setup_result;

    if ((tun_file_descriptor = open("/dev/net/tun", O_RDWR)) < 0) 
    {
        perror("Cannot open /dev/net/tun");
        return tun_file_descriptor;
    }

    memset(&interface_request, 0, sizeof(interface_request));
    interface_request.ifr_flags = IFF_TUN | IFF_NO_PI;
    strcpy(interface_request.ifr_name, "tun0");

    if ((setup_result = ioctl(tun_file_descriptor, TUNSETIFF, (void *)&interface_request)) < 0) {
        perror("ioctl[TUNSETIFF]");
        close(tun_file_descriptor);
        return setup_result;
    }

    return tun_file_descriptor;
}

void execute_command(char *command) {
    printf("Executing command: %s\n", command);
    if (system(command)) {
        perror(command);
        exit(1);
    }
}

void configure_network_interface() {
    char command[1024];
    snprintf(command, sizeof(command), "ifconfig tun0 %s/16 mtu %d up", SERVER_VPN_IP, MAX_PACKET_SIZE);
    execute_command(command);
}

void setup_routing_rules() 
{
    execute_command("sysctl -w net.ipv4.ip_forward=1");
    
    char masquerade_cmd[1024];
    snprintf(masquerade_cmd, sizeof(masquerade_cmd), 
            "iptables -t nat -A POSTROUTING -s %s ! -d %s -m comment --comment 'vpndemo' -j MASQUERADE",
            VPN_NETWORK, VPN_NETWORK);
    execute_command(masquerade_cmd);
    
    char forward_cmd1[1024], forward_cmd2[1024];
    snprintf(forward_cmd1, sizeof(forward_cmd1),
            "iptables -A FORWARD -s %s -m state --state RELATED,ESTABLISHED -j ACCEPT", 
            VPN_NETWORK);
    snprintf(forward_cmd2, sizeof(forward_cmd2),
            "iptables -A FORWARD -d %s -j ACCEPT",
            VPN_NETWORK);
            
    execute_command(forward_cmd1);
    execute_command(forward_cmd2);
}

void cleanup_routing_rules() {
    char masquerade_cmd[1024];
    snprintf(masquerade_cmd, sizeof(masquerade_cmd),
            "iptables -t nat -D POSTROUTING -s %s ! -d %s -m comment --comment 'vpndemo' -j MASQUERADE",
            VPN_NETWORK, VPN_NETWORK);
    execute_command(masquerade_cmd);
    
    char forward_cmd1[1024], forward_cmd2[1024];
    snprintf(forward_cmd1, sizeof(forward_cmd1),
            "iptables -D FORWARD -s %s -m state --state RELATED,ESTABLISHED -j ACCEPT",
            VPN_NETWORK);
    snprintf(forward_cmd2, sizeof(forward_cmd2),
            "iptables -D FORWARD -d %s -j ACCEPT",
            VPN_NETWORK);
            
    execute_command(forward_cmd1);
    execute_command(forward_cmd2);
}

int setup_udp_socket(struct sockaddr *server_addr, socklen_t* addr_len) {
    struct addrinfo hints, *server_info;
    int socket_fd, socket_flags;

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    if (0 != getaddrinfo(SERVER_BIND_ADDRESS, NULL, &hints, &server_info)) {
        perror("getaddrinfo error");
        return -1;
    }

    if (server_info->ai_family == AF_INET)
        ((struct sockaddr_in *)server_info->ai_addr)->sin_port = htons(SERVER_PORT);
    else if (server_info->ai_family == AF_INET6)
        ((struct sockaddr_in6 *)server_info->ai_addr)->sin6_port = htons(SERVER_PORT);
    else {
        fprintf(stderr, "Unknown address family %d", server_info->ai_family);
        freeaddrinfo(server_info);
        return -1;
    }
    
    memcpy(server_addr, server_info->ai_addr, server_info->ai_addrlen);
    *addr_len = server_info->ai_addrlen;

    if (-1 == (socket_fd = socket(server_info->ai_family, SOCK_DGRAM, IPPROTO_UDP))) {
        perror("Cannot create socket");
        freeaddrinfo(server_info);
        return -1;
    }

    if (0 != bind(socket_fd, server_info->ai_addr, server_info->ai_addrlen)) {
        perror("Cannot bind");
        close(socket_fd);
        freeaddrinfo(server_info);
        return -1;
    }

    freeaddrinfo(server_info);

    socket_flags = fcntl(socket_fd, F_GETFL, 0);
    if (socket_flags != -1) {
        if (-1 != fcntl(socket_fd, F_SETFL, socket_flags | O_NONBLOCK))
            return socket_fd;
    }
    perror("fcntl error");

    close(socket_fd);
    return -1;
}

void handle_cleanup(int signal_number) {
    printf("Server shutting down...\n");
    if (signal_number == SIGHUP || signal_number == SIGINT || signal_number == SIGTERM) {
        cleanup_routing_rules();
        exit(0);
    }
}

void setup_signal_handlers() {
    struct sigaction signal_action;
    signal_action.sa_handler = &handle_cleanup;
    signal_action.sa_flags = SA_RESTART;
    sigfillset(&signal_action.sa_mask);

    if (sigaction(SIGHUP, &signal_action, NULL) < 0) {
        perror("Cannot handle SIGHUP");
    }
    if (sigaction(SIGINT, &signal_action, NULL) < 0) {
        perror("Cannot handle SIGINT");
    }
    if (sigaction(SIGTERM, &signal_action, NULL) < 0) {
        perror("Cannot handle SIGTERM");
    }
}

void encrypt_packet(char *plaintext, char *ciphertext, int length) {
    memcpy(ciphertext, plaintext, length);
}

void decrypt_packet(char *ciphertext, char *plaintext, int length) {
    memcpy(plaintext, ciphertext, length);
}

int main(int argc, char **argv) {
    int tun_fd = create_tun_interface();
    if (tun_fd < 0) {
        return 1;
    }

    configure_network_interface();
    setup_routing_rules();
    setup_signal_handlers();

    int udp_fd;
    struct sockaddr_storage client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    if ((udp_fd = setup_udp_socket((struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
        return 1;
    }

    char tun_buffer[MAX_PACKET_SIZE], udp_buffer[MAX_PACKET_SIZE];
    bzero(tun_buffer, MAX_PACKET_SIZE);
    bzero(udp_buffer, MAX_PACKET_SIZE);

    while (1) {
        fd_set read_fd_set;
        FD_ZERO(&read_fd_set);
        FD_SET(tun_fd, &read_fd_set);
        FD_SET(udp_fd, &read_fd_set);
        int max_fd = get_max(tun_fd, udp_fd) + 1;

        if (-1 == select(max_fd, &read_fd_set, NULL, NULL, NULL)) {
            perror("select error");
            break;
        }

        int bytes_read;
        if (FD_ISSET(tun_fd, &read_fd_set)) {
            bytes_read = read(tun_fd, tun_buffer, MAX_PACKET_SIZE);
            if (bytes_read < 0) {
                perror("read from tun_fd error");
                break;
            }

            encrypt_packet(tun_buffer, udp_buffer, bytes_read);
            printf("Writing %d bytes to UDP...\n", bytes_read);

            bytes_read = sendto(udp_fd, udp_buffer, bytes_read, 0, 
                              (const struct sockaddr *)&client_addr, client_addr_len);
            if (bytes_read < 0) {
                perror("sendto udp_fd error");
                break;
            }
        }

        if (FD_ISSET(udp_fd, &read_fd_set)) {
            bytes_read = recvfrom(udp_fd, udp_buffer, MAX_PACKET_SIZE, 0,
                                (struct sockaddr *)&client_addr, &client_addr_len);
            if (bytes_read < 0) {
                perror("recvfrom udp_fd error");
                break;
            }

            decrypt_packet(udp_buffer, tun_buffer, bytes_read);
            printf("Writing %d bytes to tun...\n", bytes_read);

            bytes_read = write(tun_fd, tun_buffer, bytes_read);
            if (bytes_read < 0) {
                perror("write tun_fd error");
                break;
            }
        }
    }

    close(tun_fd);
    close(udp_fd);
    cleanup_routing_rules();

    return 0;
}



//vpn_client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>
#include <linux/if.h>
#include <linux/if_tun.h>

#define SERVER_PORT 7000
#define MAX_PACKET_SIZE 1400
#define CLIENT_VPN_IP "10.8.0.2"

// Set your server hostname here
#define VPN_SERVER_HOST "10.10.2.46"

int get_max(int first_num, int second_num) 
{
    if (first_num > second_num) 
        return first_num;
    else 
        return second_num;
}

int create_tun_interface() {
    struct ifreq interface_request;
    int tun_file_descriptor, setup_result;

    if ((tun_file_descriptor = open("/dev/net/tun", O_RDWR)) < 0) {
        perror("Cannot open /dev/net/tun");
        return tun_file_descriptor;
    }

    memset(&interface_request, 0, sizeof(interface_request));
    interface_request.ifr_flags = IFF_TUN | IFF_NO_PI;
    strcpy(interface_request.ifr_name, "tun0");

    if ((setup_result = ioctl(tun_file_descriptor, TUNSETIFF, (void *)&interface_request)) < 0) {
        perror("ioctl[TUNSETIFF]");
        close(tun_file_descriptor);
        return setup_result;
    }

    return tun_file_descriptor;
}

void execute_command(char *command) {
    printf("Executing command: %s\n", command);
    if (system(command)) {
        perror(command);
        exit(1);
    }
}

void configure_network_interface() {
    char command[1024];
    snprintf(command, sizeof(command), "ifconfig tun0 %s/16 mtu %d up", CLIENT_VPN_IP, MAX_PACKET_SIZE);
    execute_command(command);
}

void setup_routing_rules() {
    execute_command("sysctl -w net.ipv4.ip_forward=1");
    execute_command("iptables -t nat -A POSTROUTING -o tun0 -j MASQUERADE");
    execute_command("iptables -I FORWARD 1 -i tun0 -m state --state RELATED,ESTABLISHED -j ACCEPT");
    execute_command("iptables -I FORWARD 1 -o tun0 -j ACCEPT");
    
    // Add route to VPN server via default gateway
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), 
            "ip route add %s via $(ip route show 0/0 | sed -e 's/.* via \\([^ ]*\\).*/\\1/')",
            VPN_SERVER_HOST);
    execute_command(cmd);
    
    // Split tunneling - route all traffic through VPN
    execute_command("ip route add 0/1 dev tun0");
    execute_command("ip route add 128/1 dev tun0");
}

void cleanup_routing_rules() {
    execute_command("iptables -t nat -D POSTROUTING -o tun0 -j MASQUERADE");
    execute_command("iptables -D FORWARD -i tun0 -m state --state RELATED,ESTABLISHED -j ACCEPT");
    execute_command("iptables -D FORWARD -o tun0 -j ACCEPT");
    
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "ip route del %s", VPN_SERVER_HOST);
    execute_command(cmd);
    execute_command("ip route del 0/1");
    execute_command("ip route del 128/1");
}

int setup_udp_socket(struct sockaddr *server_addr, socklen_t* addr_len) {
    struct addrinfo hints, *server_info;
    int socket_fd, socket_flags;

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    if (0 != getaddrinfo(VPN_SERVER_HOST, NULL, &hints, &server_info)) {
        perror("getaddrinfo error");
        return -1;
    }

    if (server_info->ai_family == AF_INET)
        ((struct sockaddr_in *)server_info->ai_addr)->sin_port = htons(SERVER_PORT);
    else if (server_info->ai_family == AF_INET6)
        ((struct sockaddr_in6 *)server_info->ai_addr)->sin6_port = htons(SERVER_PORT);
    else {
        fprintf(stderr, "Unknown address family %d", server_info->ai_family);
        freeaddrinfo(server_info);
        return -1;
    }
    
    memcpy(server_addr, server_info->ai_addr, server_info->ai_addrlen);
    *addr_len = server_info->ai_addrlen;

    if (-1 == (socket_fd = socket(server_info->ai_family, SOCK_DGRAM, IPPROTO_UDP))) {
        perror("Cannot create socket");
        freeaddrinfo(server_info);
        return -1;
    }

    freeaddrinfo(server_info);

    socket_flags = fcntl(socket_fd, F_GETFL, 0);
    if (socket_flags != -1) {
        if (-1 != fcntl(socket_fd, F_SETFL, socket_flags | O_NONBLOCK))
            return socket_fd;
    }
    perror("fcntl error");

    close(socket_fd);
    return -1;
}

void handle_cleanup(int signal_number) {
    printf("Client shutting down...\n");
    if (signal_number == SIGHUP || signal_number == SIGINT || signal_number == SIGTERM) {
        cleanup_routing_rules();
        exit(0);
    }
}

void setup_signal_handlers() {
    struct sigaction signal_action;
    signal_action.sa_handler = &handle_cleanup;
    signal_action.sa_flags = SA_RESTART;
    sigfillset(&signal_action.sa_mask);

    if (sigaction(SIGHUP, &signal_action, NULL) < 0) {
        perror("Cannot handle SIGHUP");
    }
    if (sigaction(SIGINT, &signal_action, NULL) < 0) {
        perror("Cannot handle SIGINT");
    }
    if (sigaction(SIGTERM, &signal_action, NULL) < 0) {
        perror("Cannot handle SIGTERM");
    }
}

void encrypt_packet(char *plaintext, char *ciphertext, int length) {
    // TODO: Implement real encryption
    memcpy(ciphertext, plaintext, length);
}

void decrypt_packet(char *ciphertext, char *plaintext, int length) {
    // TODO: Implement real decryption
    memcpy(plaintext, ciphertext, length);
}

int main(int argc, char **argv) {
    int tun_fd = create_tun_interface();
    if (tun_fd < 0) {
        return 1;
    }

    configure_network_interface();
    setup_routing_rules();
    setup_signal_handlers();

    int udp_fd;
    struct sockaddr_storage server_addr;
    socklen_t server_addr_len = sizeof(server_addr);

    if ((udp_fd = setup_udp_socket((struct sockaddr *)&server_addr, &server_addr_len)) < 0) {
        return 1;
    }

    char tun_buffer[MAX_PACKET_SIZE], udp_buffer[MAX_PACKET_SIZE];
    bzero(tun_buffer, MAX_PACKET_SIZE);
    bzero(udp_buffer, MAX_PACKET_SIZE);

    while (1) {
        fd_set read_fd_set;
        FD_ZERO(&read_fd_set);
        FD_SET(tun_fd, &read_fd_set);
        FD_SET(udp_fd, &read_fd_set);
        int max_fd = get_max(tun_fd, udp_fd) + 1;

        if (-1 == select(max_fd, &read_fd_set, NULL, NULL, NULL)) {
            perror("select error");
            break;
        }

        int bytes_read;
        if (FD_ISSET(tun_fd, &read_fd_set)) {
            bytes_read = read(tun_fd, tun_buffer, MAX_PACKET_SIZE);
            if (bytes_read < 0) {
                perror("read from tun_fd error");
                break;
            }

            encrypt_packet(tun_buffer, udp_buffer, bytes_read);
            printf("Writing %d bytes to UDP...\n", bytes_read);

            bytes_read = sendto(udp_fd, udp_buffer, bytes_read, 0,
                              (const struct sockaddr *)&server_addr, server_addr_len);
            if (bytes_read < 0) {
                perror("sendto udp_fd error");
                break;
            }
        }

        if (FD_ISSET(udp_fd, &read_fd_set)) {
            bytes_read = recvfrom(udp_fd, udp_buffer, MAX_PACKET_SIZE, 0,
                                (struct sockaddr *)&server_addr, &server_addr_len);
            if (bytes_read < 0) {
                perror("recvfrom udp_fd error");
                break;
            }

            decrypt_packet(udp_buffer, tun_buffer, bytes_read);
            printf("Writing %d bytes to tun...\n", bytes_read);

            bytes_read = write(tun_fd, tun_buffer, bytes_read);
            if (bytes_read < 0) {
                perror("write tun_fd error");
                break;
            }
        }
    }

    close(tun_fd);
    close(udp_fd);
    cleanup_routing_rules();

    return 0;
}