1. Ethernet headers: destination MAC - 6 bytes, Source MAC - 6 bytes Ethernet Type - 2 bytes: Ethernet headers for ARP - 0x0806, IPv4 - 0x0800

For ICMP - checksum: 0x5576, Status: Good

2. To delete an ARP entry: sudo arp -d <ip_address>