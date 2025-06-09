# Setting Up a GRE Tunnel and Routing Between Physical Hosts and Their VMs

## Scenario
- My physical host and a friend's physical host are connected over a GRE tunnel.
- Both hosts have VMs in host-only network mode.
- Goal: Enable communication between my VM and my friend's VM through the tunnel.

## Steps to Set Up the Tunnel

### **1. Configured the GRE Tunnel**

#### **My Host**
```bash
sudo ip tunnel add gre1 mode gre remote 10.10.2.60 local 10.10.1.51 ttl 255
sudo ip addr add 10.0.0.2/30 dev gre1
sudo ip link set gre1 up
```

#### **Friend's Host**
```bash
sudo ip tunnel add gre1 mode gre remote 10.10.1.51 local 10.10.2.60 ttl 255
sudo ip addr add 10.0.0.1/30 dev gre1
sudo ip link set gre1 up
```

### **2. Enabled IP Forwarding**
On both physical hosts:

```bash
sudo sysctl -w net.ipv4.ip_forward=1
```
## Setting Up Routing

### **1. Added Routes for the VMs**

#### **On my Host**
Added a route to my friend’s VM subnet via the tunnel:

```bash
sudo ip route add 192.168.57.0/24 via 10.0.0.1 dev gre1
```

#### **On Friend’s Host**
Added a route to my VM subnet via the tunnel:

```bash
sudo ip route add 192.168.56.0/24 via 10.0.0.2 dev gre1
```

### **2. Verified Tunnel Connectivity**
Tested connectivity between the tunnel endpoints:

```bash
ping 10.0.0.1  # From my host
ping 10.0.0.2  # From my friend's host
```

## Firewall 

### **1. Ensured Firewalls Allow Forwarded Traffic**
On both hosts, temporarily disabled firewalls for testing:

```bash
sudo iptables -F
sudo iptables -X
sudo iptables -P FORWARD ACCEPT
sudo iptables -P INPUT ACCEPT
sudo iptables -P OUTPUT ACCEPT
```

## Testing Connectivity

1. **Pinged Friend’s VM**:
   From my VM (`192.168.56.101`):

   ```bash
   ping 192.168.57.3
   ```

2. **Pinged my VM**:
   From my friend’s VM (`192.168.57.3`):

   ```bash
   ping 192.168.56.101
   ```

