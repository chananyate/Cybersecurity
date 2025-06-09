import socket
import ipaddress
from threading import Thread


def scan_port(ip, port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.settimeout(1)
    result = sock.connect_ex((ip, port))
    if result == 0:
        print(f"Device {ip} has port {port} open")
    sock.close()


def scan_network(network, port):
    threads = []  # List to store threads for concurrent scanning
    # Iterates through all hosts in the network range
    for ip in network.hosts():  # .hosts() returns all usable IP addresses in the range (excluding network and broadcast IPs)
        # Creates a new thread for each device to scan the specified port.
        # This approach works well for smaller subnets like /24.
        # For larger subnets (with many hosts), using too many threads (and sockets) might exhaust system resources.
        thread = Thread(target=scan_port, args=(str(ip), port))
        thread.start()
        threads.append(thread)  # Stores the thread in the list so we can later ensure all threads complete before continuing

    # Waits for all threads to finish to ensure synchronization.
    for thread in threads:
        thread.join()


def main():
    ip = input("Enter the IP address to check: ")
    subnet = input("Enter subnet mask (press enter for /24, or specify /16, /32 etc.): ")
    if not subnet:
        subnet = '/24'  # Default is /24 if no subnet is specified
    # Create an IPv4 network object from the provided IP address and subnet mask.
    # The 'strict=False' argument allows the use of a host address (like 192.168.1.166) as if it were a network address.
    # Normally, the network address (like 192.168.1.0) would be expected for creating a network range,
    # but setting strict=False enables the inclusion of host addresses in the network creation.
    network = ipaddress.IPv4Network(f"{ip}{subnet}", strict=False)
    print(f"Scanning network {network} for open port...")
    port = int(input("Enter the port number to check: "))
    scan_network(network, port)

if __name__ == "__main__":
    main()
