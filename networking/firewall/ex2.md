# Exercise 2: Reject HTTP Requests and Only Allow HTTPS

## Objective

In this exercise, the goal is to block all HTTP requests (port 80) and only allow HTTPS traffic (port 443) using `iptables`.

## Steps to Achieve the Objective

### 1. Block HTTP (Port 80) Requests

First, we block incoming HTTP traffic on port 80. This is done by rejecting any TCP connection attempts to port 80.

```bash
sudo iptables -A INPUT -p tcp --dport 80 -j REJECT
sudo iptables -A INPUT -p tcp --dport 443 -j ACCEPT	
sudo iptables -A OUTPUT -p tcp --dport 80 -j REJECT
sudo iptables -A OUTPUT -p tcp --dport 443 -j ACCEPT	
```
### 2. Check Updated Rules

```bash
sudo iptables -L --line-numbers
```

### 3. Test Configuration

```bash
curl http://httpforever.com
```