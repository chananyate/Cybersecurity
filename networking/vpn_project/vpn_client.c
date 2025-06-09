#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <signal.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include "network_functions.h"

#define SERVER_PORT 7000
#define MAX_PACKET_SIZE 1400
#define CLIENT_VPN_IP "10.8.0.2"

#define VPN_SERVER_HOST "10.10.0.194"

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

    if ((setup_result = ioctl(tun_file_descriptor, TUNSETIFF, (void *)&interface_request)) < 0) 
    {
        perror("ioctl[TUNSETIFF]");
        close(tun_file_descriptor);
        return setup_result;
    }

    return tun_file_descriptor;
}

void execute_command(char *command) 
{
    printf("Executing command: %s\n", command);
    if (system(command)) 
    {
        perror(command);
        exit(1);
    }
}

void configure_network_interface() 
{
    char command[1024];
    snprintf(command, sizeof(command), "ifconfig tun0 %s/16 mtu %d up", CLIENT_VPN_IP, MAX_PACKET_SIZE);
    execute_command(command);
}

void setup_routing_rules() 
{
    /*routes traffic between the vpn tunnel (tun0) and other network interfaces*/
    execute_command("sysctl -w net.ipv4.ip_forward=1");
    /*enables nat on the client side to translate the source ip of outgoing packets from the vpn to the ip of the outgoing interface (like wlp2s0)*/
    execute_command("iptables -t nat -A POSTROUTING -o tun0 -j MASQUERADE");
    /*ensures that packets belonging to already established connections (like responses from remote servers) can flow freely back to the client.
      -I means Insert at beginning of iptables rule chain, while -A means Append to the end*/
    execute_command("iptables -I FORWARD 1 -i tun0 -m state --state RELATED,ESTABLISHED -j ACCEPT");
    /*ensures that any packets originating from the client can be sent over the vpn tunnel.*/
    execute_command("iptables -I FORWARD 1 -o tun0 -j ACCEPT");
    
    /*adds route to vpn server via default gateway*/
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), 
            "ip route add %s via $(ip route show 0/0 | sed -e 's/.* via \\([^ ]*\\).*/\\1/')",
            VPN_SERVER_HOST);
    execute_command(cmd);
    
    /*split tunneling: add routes to ensure all IPv4 traffic (0.0.0.0/1 and 128.0.0.0/1) is routed through the VPN tunnel (tun0).*/
    execute_command("ip route add 0/1 dev tun0");
    execute_command("ip route add 128/1 dev tun0");
}

void cleanup_routing_rules() 
{
    execute_command("iptables -t nat -D POSTROUTING -o tun0 -j MASQUERADE");
    execute_command("iptables -D FORWARD -i tun0 -m state --state RELATED,ESTABLISHED -j ACCEPT");
    execute_command("iptables -D FORWARD -o tun0 -j ACCEPT");
    
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "ip route del %s", VPN_SERVER_HOST);
    execute_command(cmd);
    execute_command("ip route del 0/1");
    execute_command("ip route del 128/1");
}

int setup_udp_socket(struct sockaddr *server_addr, socklen_t* addr_len) 
{
    int socket_fd = create_udp_socket();
    if (socket_fd < 0) 
        return -1;  

    struct sockaddr_in addr = setup_address(VPN_SERVER_HOST, SERVER_PORT);  
    memcpy(server_addr, &addr, sizeof(addr));  
    *addr_len = sizeof(addr);  

    int socket_flags = fcntl(socket_fd, F_GETFL, 0);
    if (socket_flags != -1) 
    {
        if (fcntl(socket_fd, F_SETFL, socket_flags | O_NONBLOCK) != -1) 
            return socket_fd;  
    }
    
    perror("fcntl error");
    close(socket_fd);
    return -1;
}


void handle_cleanup(int signal_number) 
{
    printf("Client shutting down...\n");
    if (signal_number == SIGHUP || signal_number == SIGINT || signal_number == SIGTERM) 
    {
        cleanup_routing_rules();
        exit(0);
    }
}

void setup_signal_handlers() 
{
    struct sigaction signal_action;
    signal_action.sa_handler = &handle_cleanup;
    signal_action.sa_flags = SA_RESTART;
    sigfillset(&signal_action.sa_mask);

    if (sigaction(SIGHUP, &signal_action, NULL) < 0) 
        perror("Cannot handle SIGHUP");
    
    if (sigaction(SIGINT, &signal_action, NULL) < 0) 
        perror("Cannot handle SIGINT");
    
    if (sigaction(SIGTERM, &signal_action, NULL) < 0) 
        perror("Cannot handle SIGTERM");
}


