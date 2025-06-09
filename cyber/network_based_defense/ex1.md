# Suricata Installation and Configuration Guide


## 1. Extracting Suricata Source Code
After downloading Suricata from their website, I've extracted it:

```bash
tar -xvf suricata-7.0.8.tar.gz
cd suricata-7.0.8
```

## 2. Installing Dependencies
I installed the required system packages and development libraries:

```bash
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    libpcap-dev \
    libpcre3-dev \
    libpcre2-dev \
    libyaml-dev \
    libjansson-dev \
    libhtp-dev \
    libmagic-dev \
    zlib1g-dev \
    pkg-config \
    libnet1-dev \
    liblua5.3-dev \
    python3-dev \
    rustc \
    cargo \
    liblz4-dev
```

## 3. Configuration
I configured Suricata with appropriate installation paths:

```bash
./configure --prefix=/usr/local --sysconfdir=/etc
```

This configuration:
- Sets the main installation directory to `/usr/local`
- Places configuration files in `/etc/suricata`
- Enables default features and capabilities


## 4. Compilation and Installation
I compiled the source code:

```bash
make
```

I installed Suricata and its components:

```bash
sudo make install-full
```

The `install-full` command:
- Installs the binary to `/usr/local/bin`
- Installs configuration files to `/etc/suricata`
- Sets up default rules and configuration

## 5. Post-Installation Verification
I verified the installation:

```bash
suricata --build-info
```

## 6. Basic Configuration
### Main Configuration File
The main configuration file is located at `/etc/suricata/suricata.yaml`. Key sections include:
- Network interface settings
- Rule paths
- Logging options
- Performance tuning

### Starting Suricata
I started Suricata in IDS mode on a specific interface:

```bash
sudo /usr/local/bin/suricata -c /etc/suricata/suricata.yaml -i wlp2s0
```

## 7. Rule Management
### Adding Custom Rules
1. I created a local rules file:

```bash
sudo nano /var/lib/suricata/local.rules
```

2. I added custom rules:

```text
alert icmp any any -> any any (msg:"ICMP ping detected"; sid:1000003; rev:1;)

alert tcp $HOME_NET any -> any 80 (msg:"HTTP traffic detected"; sid:1000002; rev:1;)
```
Explanation for first rule:
**This rule generates an alert whenever ICMP traffic is detected, regardless of source or destination**
1. alert:
   - Specifies the action Suricata should take when the rule matches. In this case, it generates an alert.

2. icmp:
   - The protocol to inspect. This rule applies specifically to ICMP traffic.

3. any any -> any any:
   - Source: Matches traffic from any source IP and any source port (although ICMP does not use ports).
   - ->: Indicates the direction of traffic (from source to destination).
   - Destination: Matches traffic to any destination IP and any destination port.

4. (msg:"ICMP ping detected"; sid:1000003; rev:1;):
   - msg: The message included in the alert to describe what was detected ("ICMP ping detected").
   - sid: The signature ID (SID), uniquely identifying the rule. Custom rules use SIDs >= 1000000.
   - rev: The revision number of the rule. Increment it when modifying the rule to track changes.

## 8. Make sure local.rules file is added to the list of rules:
default-rule-path: /usr/local/etc/suricata/rules

rule-files:
  - suricata.rules
  - /var/lib/suricata/local.rules



## 9. Testing the Installation

I monitored the fast.log file for alerts:
```bash
sudo tail -f /var/log/suricata/fast.log
```

```output
01/28/2025-18:58:30.791603  [**] [1:1000002:1] HTTP traffic detected [**] [Classification: (null)] [Priority: 3] {TCP} 172.20.10.2:55222 -> 104.121.10.80:80
01/28/2025-18:58:31.112177  [**] [1:2013504:6] ET INFO GNU/Linux APT User-Agent Outbound likely related to package management [**] [Classification: Not Suspicious Traffic] [Priority: 3] {TCP} 172.20.10.2:55222 -> 104.121.10.80:80
01/28/2025-19:00:15.652262  [**] [1:1000002:1] HTTP traffic detected [**] [Classification: (null)] [Priority: 3] {TCP} 172.20.10.2:40714 -> 91.189.91.98:80
01/28/2025-19:01:48.237082  [**] [1:1000003:1] ICMP ping detected [**] [Classification: (null)] [Priority: 3] {IPv6-ICMP} fe80:0000:0000:0000:88a4:79ff:fe65:4864:134 -> ff02:0000:0000:0000:0000:0000:0000:0001:0
01/28/2025-19:01:48.241362  [**] [1:1000003:1] ICMP ping detected [**] [Cl
```

# Bypassing Suricata Rules

## Rule 1: HTTP Traffic Detection (TCP on Port 80)

### Methods:
1. **Use HTTPS (Port 443)**:
   - Switch traffic to HTTPS (encrypted). This avoids detection since the rule monitors only port 80.
2. **Change Destination Port**:
   - Configure the HTTP server to run on a non-standard port (like 8080).
   - Example:
     ```bash
     curl http://ynet.co.il.co.il:8080
     ```
3. **Encapsulation**:
   - Encapsulate HTTP traffic inside other protocols (like tunneling HTTP through VPN).


## Rule 2: ICMP Ping Detection

### Methods:
1. **Use Non-ICMP Tools**:
   - Use `tcping` or `hping3` to send TCP-based pings.
     ```bash
     hping3 -S -p 80 <target-ip>
     ```
2. **Encrypt ICMP Traffic**:
   - Encapsulate ICMP packets inside other protocols like VPNs.
3. **Change Packet Characteristics**:
   - Modify ICMP packet content or structure (like TTL or packet size).
   - Example:
     ```bash
     ping -s 1400 -t 128 <target-ip>
     ```

