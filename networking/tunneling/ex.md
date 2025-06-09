# PPPoE Setup Exercise

## Overview
This guide demonstrates setting up a PPPoE connection between client and server using `pppd` daemon and associated configurations.

### Assign a static ip to client
```bash
ip addr add 192.168.1.100/24 dev enp3so
```

### Client Configuration

**/etc/ppp/peers/myconnection**:
```myconnection
noauth
user "user1"
remotename pppoe
plugin rp-pppoe.so
enp3so 
```

**/etc/ppp/options**:
```options
lcp-echo-interval 40
lcp-echo-failure 10
local
lock
noauth
```

**/etc/ppp/pap-secrets**
```pap-secrets		
"user1" * "password1"
```

### Assign a static ip to server
```bash
ip addr add 192.168.1.101/24 dev enp1so
```

### Server Configuration

**/etc/ppp/pppoe-server-options**:
```pppoe-server-options
noauth
local
lcp-echo-interval 40
lcp-echo-failure 10
```

**/etc/ppp/pap-secrets**
```pap-secrets
"user1" * "password1"
```

## Start the Connection

### Server Side
```bash
sudo pppoe-server -I <interface_name> -L 192.168.1.101 -R 192.168.1.102 - 192.168.1.110
```

### Client Side
```bash
sudo pppd call myconnection debug
```

## Verification
Check connection status:
```bash
ip addr show ppp0
```

## Troubleshooting
1. Check logs: `sudo tail -f /var/log/syslog`
2. Verify Ethernet interface name
3. Confirm `pppd` process is running
4. Check firewall and routing configurations


