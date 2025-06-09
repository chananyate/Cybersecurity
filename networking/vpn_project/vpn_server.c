#include <stdio.h>      /*printf*/
#include <stdlib.h>     /*system()*/
#include <string.h>     /*memset*/
#include <unistd.h>     /*read(), write()*/
#include <sys/socket.h> /*sockaddr_storage*/
#include <sys/types.h>  /*socklen_t*/
#include <sys/stat.h>   /*file status*/
#include <sys/ioctl.h>  /*I/O control operations - needed for network interface configuration*/
#include <fcntl.h>      /*file control - open(), fcntl()*/
#include <signal.h>     /*signal handling - for cleanup on program termination*/
#include <linux/if.h>   /*linux network interface definitions*/
#include <linux/if_tun.h> /*linux tun device definitions*/
#include "network_functions.h"

#define SERVER_PORT 7000    /*udp port number for vpn traffic*/
#define MAX_PACKET_SIZE 1400
#define SERVER_BIND_ADDRESS "0.0.0.0"   /*server bind address - allows the server to accept connections from any available network interface to ensure flexibility and portability across different network environments and interfaces.*/
#define VPN_NETWORK "10.8.0.0/16"
#define SERVER_VPN_IP "10.8.0.1"


int create_tun_interface() 
{
    struct ifreq interface_request;  /*struct ifreq is a structure used to configure network interfaces*/
    int tun_file_descriptor, setup_result;

    if ((tun_file_descriptor = open("/dev/net/tun", O_RDWR)) < 0) /*opens tun device in read-write mode, returns -1 on error*/
    {   
        perror("Cannot open /dev/net/tun");
        return tun_file_descriptor;
    }

    memset(&interface_request, 0, sizeof(interface_request));  /*zeros out interface request structure*/
    interface_request.ifr_flags = IFF_TUN | IFF_NO_PI;  /*IFF_TUN: creates tun device (ip level), IFF_NO_PI: no packet information header*/
    strcpy(interface_request.ifr_name, "tun0");         /*sets the name of the interface to "tun0"*/

    /*the ioctl system call with the TUNSETIFF command "attaches" the file descriptor (which we got from "open()") to a new tun interface, based on the ifreq structure.
      this creates the virtual network interface in the kernel and ties it to the program via the file descriptor, so that the program can send and receive packets through the file descriptor.*/
    if ((setup_result = ioctl(tun_file_descriptor, TUNSETIFF, (void *)&interface_request)) < 0) 
    {
        perror("ioctl[TUNSETIFF]");
        close(tun_file_descriptor);
        return setup_result;
    }

    return tun_file_descriptor;
}

/*the system function is used to execute a shell command. It takes the command string as an argument and runs it in the default shell. Returns 0 on success.*/
void execute_command(char *command) 
{
    printf("Executing command: %s\n", command);
    if (system(command)) 
    {
        perror(command);
        exit(1);
    }
}

/*configures a network interface (tun0) with an ip address, subnet mask, and maximum transmission unit (MTU), 
  and then brings it "up" (activates it)*/
void configure_network_interface() 
{
    char command[1024];
    snprintf(command, sizeof(command), "ifconfig tun0 %s/16 mtu %d up", SERVER_VPN_IP, MAX_PACKET_SIZE);
    execute_command(command);
}
 
/*configures the network routing and firewall rules for the vpn setup*/  
void setup_routing_rules() 
{
    execute_command("sysctl -w net.ipv4.ip_forward=1");  /*enables ip forwarding*/
    
    char masquerade_cmd[1024];

    /*sets up NAT for the vpn network. It ensures that packets originating from the vpn network but not destined 
      for the vpn network are masqueraded (their source ip is replaced with the server's ip). 
      POSTROUTING: the rule is applied after routing, just before packets leave the server.*/
    snprintf(masquerade_cmd, sizeof(masquerade_cmd), "iptables -t nat -A POSTROUTING -s %s ! -d %s -j MASQUERADE", VPN_NETWORK, VPN_NETWORK);

    execute_command(masquerade_cmd);
    
    char forward_cmd1[1024], forward_cmd2[1024];

    /*allows packets originating from the vpn network that are part of an established 
      or related connection to be forwarded to their destination.*/
    snprintf(forward_cmd1, sizeof(forward_cmd1),
            "iptables -A FORWARD -s %s -m state --state RELATED,ESTABLISHED -j ACCEPT", 
            VPN_NETWORK);
    /*allows packets destined for the vpn network to be forwarded to the vpn clients.*/
    snprintf(forward_cmd2, sizeof(forward_cmd2),
            "iptables -A FORWARD -d %s -j ACCEPT",
            VPN_NETWORK);
            
    execute_command(forward_cmd1);
    execute_command(forward_cmd2);
}

/*reverses the changes made by setup_routing_rules function*/
void cleanup_routing_rules() 
{
    char masquerade_cmd[1024];
    snprintf(masquerade_cmd, sizeof(masquerade_cmd), "iptables -t nat -A POSTROUTING -s %s ! -d %s -j MASQUERADE", VPN_NETWORK, VPN_NETWORK);
 
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

/*sets up a udp socket to send and receive data over the network*/
int setup_udp_socket(struct sockaddr *server_addr, socklen_t* addr_len) 
{
    /*creates a udp socket*/
    int socket_fd = create_udp_socket();
    if (socket_fd < 0) 
        return -1;  

    /*sets up the server address*/
    struct sockaddr_in addr = setup_address(SERVER_BIND_ADDRESS, SERVER_PORT);
    memcpy(server_addr, &addr, sizeof(addr));  /*copies the server address to the provided struct*/
    *addr_len = sizeof(addr);  /*sets the address length*/

    /*binds the socket to the address and port*/
    setup_binding(socket_fd, SERVER_PORT);

    /*retrieves the current file status flags set on the socket (like O_RDWR), and then adds on non-blocking mode using bitwise OR. In this mode system calls 
      like read() and write() return immediately if the socket_fd isn't available, rather than blocking the program's execution.*/
    int socket_flags = fcntl(socket_fd, F_GETFL, 0);
    if (socket_flags != -1) 
    {
        if (fcntl(socket_fd, F_SETFL, socket_flags | O_NONBLOCK) != -1) 
            return socket_fd;  /*returns the socket file descriptor if successful*/
    }
    
    /*error handling if the socket cannot be set to non-blocking*/
    perror("fcntl error");
    close(socket_fd);
    return -1;
}


/*handles specific signals (SIGHUP, SIGINT, and SIGTERM) and cleans up resources before the server shuts down.*/
void handle_cleanup(int signal_number) 
{
    printf("Server shutting down...\n");
    if (signal_number == SIGHUP || signal_number == SIGINT || signal_number == SIGTERM) 
    {
        cleanup_routing_rules();
        exit(0);  /*success*/
    }
}

void setup_signal_handlers() 
{
    struct sigaction signal_action;
    signal_action.sa_handler = &handle_cleanup;

    /*Ensures that interrupted system calls (like read or write) are automatically restarted rather than failing with an error*/
    signal_action.sa_flags = SA_RESTART;

    /*populates the sa_mask field with all signals, preventing any other signals from interrupting the execution of the handler.*/
    sigfillset(&signal_action.sa_mask);

    /*associates the handlers with signals*/
    if (sigaction(SIGHUP, &signal_action, NULL) < 0) 
        perror("Cannot handle SIGHUP");   /*sent when the terminal or process is closed*/
    
    if (sigaction(SIGINT, &signal_action, NULL) < 0) 
        perror("Cannot handle SIGINT");   /*sent when the user interrupts the program (like Ctrl+C)*/
    
    if (sigaction(SIGTERM, &signal_action, NULL) < 0) 
        perror("Cannot handle SIGTERM");  /*a polite request to terminate the process.*/
    
}

