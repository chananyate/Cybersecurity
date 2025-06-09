# VPN Program Flow Overview

## Overview
This program establishes a virtual network tunnel using the TUN interface to handle raw IP packets. It encapsulates these packets into UDP for transmission to a remote endpoint over the network. Incoming UDP packets are decapsulated and injected back into the TUN interface as raw IP packets. By bridging the local virtual network and the remote endpoint, the program creates a communication channel. It leverages the select() system call for efficient monitoring of file descriptors (for read and write), which ensures minimal CPU usage by avoiding constant polling and responding only to actual activity.


## 1. **Configures the Network Interface (TUN Interface)**
- **Purpose**: The TUN interface acts as a virtual network interface. It allows user-space programs to handle packets as if they are being sent and received from a real network device.
- **Relevant Function**: `create_tun_interface()`
- **What Happens**:
  - A TUN interface is created.
  - The file descriptor (`tun_fd`) is obtained, which is used to interact with the virtual interface.

---

## 2. **Configures the TUN Interface**
- **Purpose**: Once the TUN interface is created, it is configured with the appropriate network settings, such as IP addresses and MTU.
- **Relevant Function**: `configure_network_interface()`
- **What Happens**:
  - The program uses `ioctl()` to configure the TUN device settings.

---

## 3. **Sets Up Routing Rules**
- **Purpose**: Adds necessary routing rules to direct specific traffic to the TUN interface.
- **Relevant Function**: `setup_routing_rules()`
- **What Happens**:
  - Enables IP forwarding on the server.
  - Configures NAT and forwarding rules using `iptables` for communication between the VPN and external networks.
  - Done using system commands to manipulate the kernel's routing table.

---

## 4. **Sets Up Signal Handlers**
- **Purpose**: Ensures the program can handle cleanup tasks (like resetting routing rules, closing sockets) when interrupted or terminated.
- **Relevant Function**: `setup_signal_handlers()`
- **What Happens**:
  - Signals such as `SIGINT` and `SIGTERM` are captured, and custom handlers are defined.
  - On termination, cleanup routines are triggered (like `cleanup_routing_rules()`).

---

## 5. **Sets Up UDP Socket**
- **Purpose**: Establishes a UDP socket for communication with the remote server or client.
- **Relevant Function**: `setup_udp_socket()`
- **What Happens**:
  - A UDP socket is created and bound (in server mode) to a specific address and port.
  - `udp_fd` is obtained, representing the socket file descriptor.
  - The socket is ready to send and receive packets.

---

## 6. **Waits for an FD to Be Ready (Read/Write)**
- **Purpose**: Enters the main loop to monitor both the TUN interface and the UDP socket for activity.
- **How It Works**:
  - `select()` is used to wait for one or more file descriptors (`tun_fd` and `udp_fd`) to become ready for reading.
  - If `tun_fd` is ready, it means there’s data from the TUN interface to be sent over UDP.
  - If `udp_fd` is ready, it means there’s data from the UDP socket to be sent to the TUN interface.
- **What Happens in the Loop**:
  - **TUN to UDP**:
    Data from the TUN interface is read into `tun_buffer` and sent over the UDP socket.
  - **UDP to TUN**:
    Data from the UDP socket is read into `udp_buffer` and written to the TUN interface.

---

## 7. **Cleanup and Exit**
- **Purpose**: When the program terminates (due to an error or signal for example), it ensures proper cleanup.
- **Relevant Functions**:
  - Close file descriptors (`tun_fd`, `udp_fd`).
  - Reset routing rules (`cleanup_routing_rules()`).

---

