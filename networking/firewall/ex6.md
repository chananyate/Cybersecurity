# Exercise 6: Networking Configuration and SSH Tunneling

This exercise involves setting up a guest machine to run on the host using a Host-only Adapter, configuring network address translation (NAT), running a web server, and configuring SSH tunneling for secure browsing. The steps also include blocking access to the guest server from the host firewall.

## Set Up Guest Machine with Host-only Adapter

1. **Configured the Virtual Machine**:
   - Ensured that the guest machine is configured to use the **Host-only Adapter**
   - The Host-only Adapter allows the guest to communicate with the host and other VMs on the same host network but does not provide direct internet access
   
2. **Network Configuration**:
   - Assigned a static IP to the guest within the host's network range. For example:
     - Guest IP: `192.168.56.2`
     - Host IP: `192.168.56.1`

## Implemented NAT with iptables

To enable internet access for the guest machine, configured **Network Address Translation (NAT)** using **iptables** on the host.

1. **Enable IP Forwarding**:
   ```bash
   sudo sysctl -w net.ipv4.ip_forward=1
   ```
   This allows the host to forward network traffic between the guest and the internet.

2. **Configured iptables for NAT**:
   - Created a NAT rule to forward packets from the guest to the internet:
   ```bash
   sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
   ```
   Replaced `eth0` with the network interface connected to the internet.

3. **Tested Internet Access**:
   ```bash
   ping google.com
   ```

## Ran HTTP Server (Apache2) on the Guest

1. **Installed Apache on the Guest**:
   ```bash
   sudo apt update
   sudo apt install apache2
   ```

2. **Started Apache Service**:
   ```bash
   sudo systemctl enable apache2
   sudo systemctl start apache2
   ```

3. **Forward port 80 (HTTP) to another student's server (ip 10.10.2.60)**
   
   **On Host**

   ```bash
   sudo iptables -t nat -A PREROUTING -p tcp --dport 8080 -j DNAT --to-destination 10.10.2.60:80
   ```

   **In VM browser**
   ```browser
   http://10.10.2.60:80
   ```

## Blocked Access to Guest Server Using Host Firewall

1. **Blocked Guest Server on Host**:
   ```bash
   sudo iptables -A INPUT -p tcp --dport 80 -d 192.168.56.2 -j REJECT
   ```
   This prevents the host from accessing the guest's Apache server (port 80).

2. **Verified Block**:
   - Tried accessing `http://192.168.56.2` from the host's browser. The connection should be blocked.

## Enabled Browsing to the Server Using SSH Tunneling

### The SSH tunnel forwards traffic from port 8080 on the host machine to port 80 on the VM, allowing secure access to the VM’s web server. This enables us to access the Apache server running on the VM as if it were running locally on the host, using `http://localhost:8080` in the browser.


1. **Set Up SSH Tunnel**:
   ```bash
   ssh -L 8080:localhost:80 ubuntu@192.168.56.2
   ```
   This created a tunnel that forwards port `8080` on the host to port `80` on the guest.

2. **Tested the Tunnel**:
   - In the host's browser, navigateed to `http://localhost:8080`
   - This displays the web page served by the guest's Apache server, even though direct access to the guest's IP is blocked by the firewall


