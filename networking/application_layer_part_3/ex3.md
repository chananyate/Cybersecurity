# DNS Zone Transfer Exercise

## Objective
Set up a DNS server with a master-slave configuration, verify zone transfers, and troubleshoot issues during the process.

## Steps

### 1. Install BIND9
- Install BIND9 on both master and slave servers:
  ```bash
  sudo apt update
  sudo apt install bind9 bind9utils
  ```

### 2. Configure the Master Server
1. Edit the **named.conf.local** file on the master:
   ```bash
   sudo nano /etc/bind/named.conf.local
   ```

2. Add the zone configuration:
   ```bash
   zone "chananya.templeman" {
       type master;
       file "/etc/bind/db.chananya.templeman";
       allow-transfer { 192.168.1.172; };  # IP of the slave server
   };
   ```

3. Create the zone file:
   ```bash
   sudo cp /etc/bind/db.local /etc/bind/db.chananya.templeman
   sudo nano /etc/bind/db.chananya.templeman
   ```

4. Populate the zone file:
   ```txt
   $TTL    86400
   @       IN      SOA     ns1.chananya.templeman. chananya.te.gmail.com. (
                       2024010102 ; Serial
                       3600       ; Refresh
                       1800       ; Retry
                       1209600    ; Expire
                       86400 )    ; Minimum TTL
   ;
   @       IN      NS      ns1.chananya.templeman.
   ns1     IN      A       192.168.1.166
   admin   IN      A       192.168.1.167
   backup  IN      A       192.168.1.168
   ```

5. Check the configuration:
   ```bash
   sudo named-checkconf /etc/bind/named.conf
   sudo named-checkzone chananya.templeman /etc/bind/db.chananya.templeman
   ```

6. Restart BIND9:
   ```bash
   sudo systemctl restart bind9
   ```

### 3. Configure the Slave Server
1. Edit the **named.conf.local** file on the slave:
   ```bash
   sudo nano /etc/bind/named.conf.local
   ```

2. Add the zone configuration:
   ```bash
   zone "chananya.templeman" {
       type slave;
       file "/var/cache/bind/db.chananya.templeman";
       masters { 192.168.1.166; };  # IP of the master server
   };
   ```

3. Restart BIND9 on the slave:
   ```bash
   sudo systemctl restart bind9
   ```

4. Check the logs to confirm the transfer:
   ```bash
   sudo tail -f /var/log/syslog
   ```

### 4. Verify Zone Transfer
- Use `dig` to query the slave server:
  ```bash
  dig @192.168.1.172 chananya.templeman AXFR
  ```

## Troubleshooting

### 1. Zone Not Transferring
- Ensure `allow-transfer` is set correctly on the master.
- Check the logs on both master and slave:
  ```bash
  sudo tail -f /var/log/syslog
  ```

### 2. Manual Zone Transfer
- Use `rndc` to manually refresh:
  ```bash
  sudo rndc refresh chananya.templeman
  ```

### 3. Missing Records in Slave
- Verify the zone file on the master for completeness.
- Ensure the serial number is updated in the SOA record.

### 4. Test the Slave Server
- Use `dig` to query the slave:
  ```bash
  dig @192.168.1.172 chananya.templeman
  ```

## Notes
- Restart the master server after changing configuration:
  ```bash
  sudo systemctl restart bind9
  ```
- Ensure BIND9 is running on both servers during transfers:
  ```bash
  sudo systemctl status bind9
  ```
