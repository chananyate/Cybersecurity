# Investigating Network Problems and Potential Cyber Attacks

## Information Needed

### 1. **Network Configuration**
   - **Check IP addresses**, **routes**, and **subnet masks** to ensure the network is set up correctly.
   
### 2. **Logs**
   - **Firewall Logs**: Look for unusual traffic patterns or blocked connections.
   - **Syslog**: Check for network-related errors or suspicious activities.
   - **IDS/IPS Logs**: If using an intrusion detection system (e.g., **Suricata** or **Snort**), review for any detected attacks.

### 3. **Network Performance Metrics**
   - Use tools like **ping**, **traceroute**, **netstat**, or **iftop** to analyze the network's health and performance.
   
### 4. **Suspicious Traffic Patterns**
   - Look for anomalies such as:
     - Sudden increase in traffic
     - Unexplained connections or traffic from unknown IPs

## Investigation Tools

### 1. **Tcpdump or Wireshark**
   - Use these tools to capture and analyze network packets to inspect traffic in real-time or from saved logs.
   
### 2. **Nmap**
   - Use **nmap** for network discovery and vulnerability scanning to find open ports or misconfigured services.
   
### 3. **Fail2ban (or similar tools)**
   - Install and configure tools like **Fail2ban** to block suspicious IPs after multiple failed login attempts or detected brute-force attacks.

## Steps to Diagnose

### 1. **Verify Network Connectivity**
   - Use **ping** or **traceroute** to check if your device is reachable across the network or to external resources.

### 2. **Check for Open Ports**
   - Use **nmap** to scan for unauthorized open ports on your system. This can help identify any potential misconfigured services or vulnerabilities.

### 3. **Check IDS/IPS Logs**
   - Review **IDS/IPS logs** for any detected attacks or abnormal network activity that could indicate an intrusion.

### 4. **Analyze Packet Captures**
   - Use tools like **Wireshark** or **tcpdump** to capture network traffic and identify unusual or malicious activity, such as unexpected protocols or communication patterns.
