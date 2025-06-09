
# Reconnaissance and Scanning with Tools

1. ## DNS enumeration for Mapping Subdomains

I used `dnsenum` to map subdomains of `ilrd.co.il`. 

### dnsenum command
To map subdomains of a domain, I used the following command:
```bash
dnsenum ilrd.co.il
```
This command uses the default /usr/share/dnsenum/dns.txt file to find subdomains by querying the DNS records for `ilrd.co.il`.

I can use another dictionary file with many more entries
```bash
dnsenum -f word_list_dns_enumaration.txt ilrd.co.il
```
I can also specify a particular DNS server to use for the enumeration:
```bash
dnsenum --enum --dnsserver 62.219.91.45 ilrd.co.il
```

---

2. ## Directory Brute Forcing

I also used `dirb` to find hidden files inside a host retrieved by enum:

```bash
dirb http://172.67.147.130
```
This command tries to find directories on the target url using a the default wordlist /usr/share/dirb/wordlists/common.txt.

I can also use a diffeent dictionary 
```bash
dirb http://64.233.166.27 finaldic.txt
```
---

3. ## Nmap for Scanning Hosts

```bash
nmap -sT -Pn 142.251.9.26
```
```bash
sudo nmap -sU -Pn 142.251.9.26
```

### `-sU` vs `-sT`:
- `-sU` is used for scanning UDP ports and requires `sudo` privileges because it needs to send raw packets.
- `-sT` is used for TCP scanning and does not require `sudo` because it uses existing socket connections.
- The `-Pn` option tells `nmap` to skip host discovery and treat all hosts as online, even if they do not respond to a ping.

---

5. ## Scanning Network 

```bash
nmap -sn 10.10.0.0/22
```
### The -sn option in nmap performs a "ping scan," which only checks if hosts are up without scanning for open ports, and is therefore faster.

---

6. ## List Open Services On Host
I used `nmap` to scan hosts and find open services:

```bash
nmap -A 10.10.2.59
```
This command performs an aggressive scan, detecting open ports, services, and providing OS fingerprinting.

---

7. ## Generating All Possible Israeli Phone Numbers with Crunch

### Generating Phone Numbers Without Prefix:
```bash
crunch 10 10 0123456789 -t 05%%%%%%%%
```

### Generating Phone Numbers With Prefix:
```bash
crunch 13 13 0123456789 -t +972%%%%%%%%%
```
---
