# DNS Setup Report: Student 3 Configuration

## Project Overview
This document details the DNS zone configuration for the domain `student3.student2.student1` as part of a hierarchical DNS infrastructure project.

## Objectives
- Configure DNS zone for `student3.student2.student1`
- Set up delegation to `student4.student3.student2.student1`
- Validate DNS configuration
- Prepare for final DNS resolution chain

## Configuration Steps

### 1. Zone File Creation
Created the zone file `/etc/bind/db.student3` with the following key components:
- SOA (Start of Authority) record
- NS (Name Server) records
- Delegation configuration for `student4`

#### Zone File Example
```bash
$TTL 86400
@   IN  SOA student3.student2.student1. admin.student3.student2.student1. (
            2023121101 ; Serial
            3600       ; Refresh
            1800       ; Retry
            604800     ; Expire
            86400      ; Minimum TTL
        )
    IN  NS  ns1.student3.student2.student1.
ns1 IN  A   192.168.1.166

; Delegation for next subdomain
student4 IN NS ns1.student4.student3.student2.student1.
ns1.student4.student3.student2.student1. IN A 192.168.1.254
```

### 2. Zone Validation
Verified the zone configuration using `named-checkzone`:
```bash
sudo named-checkzone student3.student2.student1 /etc/bind/db.student3
```
- Confirmed no syntax errors

### 3. DNS Configuration Testing
Tested nameserver configuration using `dig`:
```bash
dig @192.168.1.166 student3.student2.student1 NS
```
- Verified NS record resolution
- Confirmed correct nameserver mapping

### 4. Delegation Configuration
- Added NS record for `student4`
- Used placeholder IP `192.168.1.254` for validation

### 5. Final Query Preparation
Prepared query template for final DNS resolution:
```bash
dig @<student4-dns-ip> student4.student3.student2.student1 TXT
```
