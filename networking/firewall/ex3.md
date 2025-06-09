# Exercise 3: Block Pings with `iptables`

## Objective
Block all incoming ICMP ping requests (Echo Requests) to the system using `iptables`.

## Explanation
Ping requests use the ICMP protocol to test connectivity between devices. By blocking ICMP Echo Requests, the system becomes unresponsive to ping commands. This can improve security by reducing its visibility on the network.

## Steps to Implement

1. **Blocked ICMP Echo Requests**:

   ```bash
   sudo iptables -A INPUT -p icmp --icmp-type echo-request -j DROP
   ```

2. **Verified the Rule**:

   ```bash
   sudo iptables -L --line-numbers
   ```

3. **Tested the Configuration**:
   - From another device, I tried pinging my computer, the ping failed.


