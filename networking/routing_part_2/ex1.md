
### 1. Create Two Linux Host Machines
- Install VirtualBox on each host machine.
- Create an Ubuntu virtual machine (VM) on each host and name them:
  - **Guest1**
  - **chananyavm**

### 2. Configure Guest1 with NAT Networking
- In VirtualBox, set the network adapter of Guest1 to **NAT**.
  - Navigate to **Tools -> Nat Networks**.
  - Choose **Create**.

### 3. Install Networking Tools on Guest1

   ```bash
   sudo apt install net-tools
   ```

### 4. Verify Guest1's Network Configuration
1. Check the network configuration using:
   ```bash
   ifconfig
   ```

### 5. Test Connectivity from Guest1
- Test external connectivity by pinging a public IP, such as Google's DNS server:
  ```bash
  ping 8.8.8.8
  ```
  - This succeeds, confirming internet access.

- Test connectivity to chananyavm:

    ```bash
    ping 192.168.64.2
    ```
  - **Result:** The ping fails because Guest1 (NAT) and chananyavm (other network type) are isolated.


## Explanation of Results

### Why NAT Allows Internet Access but Blocks VM-to-VM Communication
1. **Internet Access:**
   - In NAT mode, Guest1's traffic is routed through VirtualBox's NAT router, which translates the private IP to the host's public IP, enabling external connectivity.

2. **Blocked VM-to-VM Communication:**
   - By default, VirtualBox's NAT mode isolates VMs to enhance security.
   - Guest1 cannot communicate with other VMs unless they are also on the same NAT Network.

### TTL Observations
- When pinging `ynet.co.il`, the TTL values reveal how packets traverse the network:
  - **Host Machine:** Packets start with a TTL of 64 and decrease by the number of hops to the destination.
  - **chananyavm:**
    - It communicates with the world through the host, and therefore ttl decrements by 1 (48 on host, 47 on vm).
  - **Guest1:**
    - NAT rewrites the ttl so 48 on the host but 63 on vm since as far as the vm is concerned, the NAT router is the destination.


