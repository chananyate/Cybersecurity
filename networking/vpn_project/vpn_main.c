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

#define MAX_PACKET_SIZE 1400

int main() 
{
    int tun_fd = create_tun_interface();
    if (tun_fd < 0) 
        return 1;

    configure_network_interface();
    setup_routing_rules();
    setup_signal_handlers();

    int udp_fd;
    struct sockaddr_storage addr;  /*structure to store the server's and client's address information.*/
    socklen_t addr_len = sizeof(addr);  /*socklen_t is a data type used to store the size of a socket address structure.*/

    /*creates a udp socket and prepares it for communication.*/
    if ((udp_fd = setup_udp_socket((struct sockaddr *)&addr, &addr_len)) < 0) 
        return 1;

    /*defines and initializes buffers to temporarily store packet data from the tun interface (tun_buffer) and udp socket (udp_buffer).*/
    char tun_buffer[MAX_PACKET_SIZE], udp_buffer[MAX_PACKET_SIZE];
    memset(tun_buffer, 0, MAX_PACKET_SIZE);  /*sets all bytes in the tun_buffer array to zero*/
    memset(udp_buffer, 0, MAX_PACKET_SIZE);

    /*infinite loop continuously monitors the tun interface and the udp socket for activity.*/
    while (1) 
    {
        int max_fd;

        /*manages multiple file descriptors (in this case, tun_fd and udp_fd) using the select() system call, 
          which allows the program to wait for activity on one or more file descriptors.*/
        fd_set read_fd_set;
        FD_ZERO(&read_fd_set);  /*clears the fd_set struct, which is the set of file descriptors we want to monitor.*/
        FD_SET(tun_fd, &read_fd_set);  /*adds tun_fd (the file descriptor for the tun interface) to the read_fd_set, telling select() to monitor it and see if data is available to read from this interface.*/
        FD_SET(udp_fd, &read_fd_set);

        /*calculates the maximum file descriptor value. The select() call requires the maximum file descriptor value to be passed as an argument,
          since it needs to know the highest value to monitor for activity in the entire range. Example: if the max_fd is 10,
          select() will check all file descriptors from 0 to 10.*/
        max_fd = (tun_fd > udp_fd ? tun_fd : udp_fd) + 1;

        /*select() pauses the program until there is data available to read from either tun_fd or udp_fd,
          instead of continuously checking (polling) both file descriptors in a loop, which would waste cpu resources.*/ 
        if (-1 == select(max_fd, &read_fd_set, NULL, NULL, NULL)) 
        {
            perror("select error");
            break;
        }

        int bytes_read;

        /*handles data from the tun interface*/
        if (FD_ISSET(tun_fd, &read_fd_set))  /*checks if there is data available to be read from the tun interface*/
        {
            bytes_read = read(tun_fd, tun_buffer, MAX_PACKET_SIZE);
            if (bytes_read < 0) 
            {
                perror("read from tun_fd error");
                break;
            }

            memcpy(udp_buffer, tun_buffer, bytes_read);  /*copies the data from tun_buffer into udp_buffer, preparing it to be sent over the udp socket.*/
            printf("Writing %d bytes to UDP...\n", bytes_read);

            /*sends the data in udp_buffer (which is the data read from tun_fd) over the udp socket to the client address on the given port.*/
            bytes_read = sendto(udp_fd, udp_buffer, bytes_read, 0, 
                              (const struct sockaddr *)&addr, addr_len);
            if (bytes_read < 0) 
            {
                perror("sendto udp_fd error");
                break;
            }
        }

        /*handles data from the udp socket*/
        if (FD_ISSET(udp_fd, &read_fd_set)) 
        {
            /*reads the data from the udp socket into udp_buffer*/
            bytes_read = recvfrom(udp_fd, udp_buffer, MAX_PACKET_SIZE, 0,
                                (struct sockaddr *)&addr, &addr_len);
            if (bytes_read < 0) 
            {
                perror("recvfrom udp_fd error");
                break;
            }

            memcpy(tun_buffer, udp_buffer, bytes_read);  /*copies the data from udp_buffer into tun_buffer*/
            printf("Writing %d bytes to tun...\n", bytes_read);

            bytes_read = write(tun_fd, tun_buffer, bytes_read);  /*writes the data in tun_buffer to the tun interface*/
            if (bytes_read < 0) 
            {
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