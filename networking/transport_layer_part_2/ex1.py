import socket

def scan_ports(ip):
    open_ports = []  # List to store open ports
    for port in range(1, 65536):
        # Creates a socket (a channel) object for a TCP connection.
        # (AF_INET means address family of ipv4,
        # and SOCK_STREAM means a streaming socket type, which is TCP since it
        # provides a continuous, ordered, and reliable flow of data between two devices).
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.settimeout(1)  # Sets a timeout of 1 second for the connection attempt to prevent the script from waiting indefinitely.
        print(f"Scanning port {port}...")
        result = sock.connect_ex((ip, port))  # Attempts to connect to the given IP and port

        if result == 0:  # Checks if the port is open
            open_ports.append(port)  # Adds the open port to the list
            print(f"Port {port} is open")
        sock.close()
    return open_ports

def main():
    ip = input("Enter IP address to scan: ")
    print("Scanning...", ip)
    open_ports = scan_ports(ip)
    if open_ports:
        print("Open ports:", open_ports)
    else:
        print("No open ports found on", ip)

if __name__ == "__main__":
    main()
