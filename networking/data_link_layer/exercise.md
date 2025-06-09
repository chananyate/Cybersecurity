1. Network Interface Card (NIC) Information:

    IP Address: 10.10.2.44
    MAC Address: -------.
    Subnet Mask: /22

2. Packet Capture Mode:

    The network interface is capturing packets in promiscuous mode, meaning it captures all packets on the network segment, not just those addressed to the NIC itself.

3. Finding Chen's MAC Address:

    To find Chen's MAC address, the following steps were taken:
        Started Wireshark.
        Used the command ping <Chen's IP address> to generate ARP requests.
        Filtered for ARP protocols in Wireshark.
        Clicked "Edit" -> "Find Packet" -> "Display Filter" -> "String" and entered Chen's IP address.
        Found the packet that said "who has <Chen's IP address> tell 10.10.2.44".
    Chen's MAC address is -------.

4. John's MAC Address:

    ipad's MAC address is --------.

5. Blocking Network Traffic:

    There are several ways to block all network traffic from another machine on the LAN:
        a. Using administrative access to the router or firewall:
            Access the router or firewall's configuration.
            Block the target machine's IP address.
        b. Using iptables on a Linux router:
            First access the router through SSH.
            Use the following iptables commands to block incoming and outgoing traffic from the target IP address:

            sudo iptables -A INPUT -s <target_ip_address> -j DROP
            sudo iptables -A OUTPUT -d <target_ip_address> -j DROP
