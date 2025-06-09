# Exercise 5: SSH Configuration and Port Management

## Objective
Configure SSH to listen on ports 500 for a specific ip, restricting everyone else on that port.

## Steps

### 1. Added rules to ip table to listen on port 500 for a specific ip and reject all else on that port

```bash
sudo iptables -A INPUT -p tcp --dport 500 -s 10.10.0.224 -j ACCEPT

# Reject SSH on port 500 from all other IPs
sudo iptables -A INPUT -p tcp --dport 500 -j REJECT
```

### 2. Modified SSH Configuration

Edited `/etc/ssh/sshd_config`:

```bash
sudo nano /etc/ssh/sshd_config
```

Added:

```plaintext
Port 500
```

```bash
sudo systemctl disable ssh.service
sudo systemctl disable ssh.socket
sudo systemctl stop ssh.service
sudo systemctl stop ssh.socket
sudo systemctl enable ssh.service
sudo systemctl enable ssh.socket
sudo systemctl start ssh.service
```

### 4. Checked Listening Port

```bash
sudo netstat -tuln | grep :500
```

### 5. Connected from another computer via SSH on Port 500 (saw connection refused) and from 10.10.0.224 (succeeded)

```bash
ssh -p 500 guest2@10.10.1.33
```

