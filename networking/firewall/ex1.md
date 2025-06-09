# Exercise 1: Block https://cnn.com Using `iptables`

## Objective
The goal of this exercise is to block access to the website `https://cnn.com` using `iptables`. This will involve blocking all IP addresses associated with `cnn.com` so that users can't access the site from the system.

## Steps

### 1. Find the IP Addresses of `cnn.com`

Used the `nslookup` command to find the IP addresses associated with `cnn.com`:

```bash
nslookup cnn.com
```

```output
Server:		127.0.0.53
Address:	127.0.0.53#53

Non-authoritative answer:
Name:	cnn.com
Address: 151.101.131.5
Name:	cnn.com
Address: 151.101.195.5
Name:	cnn.com
Address: 151.101.67.5
Name:	cnn.com
Address: 151.101.3.5
```

### 2. Block the IP Addresses Using iptables

```bash
sudo iptables -A OUTPUT -d 151.101.131.5 -j REJECT
sudo iptables -A OUTPUT -d 151.101.195.5 -j REJECT
sudo iptables -A OUTPUT -d 151.101.67.5 -j REJECT
sudo iptables -A OUTPUT -d 151.101.3.5 -j REJECT
```

