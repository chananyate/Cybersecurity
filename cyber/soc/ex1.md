# Syslog Logging Exercise

## Overview

In this exercise, I created a C program that logs messages to the syslog, configured the client system to forward logs with severity greater (lower number) than `warning` to a remote syslog server, and set up a VM as the syslog server to collect these logs.

## Steps

### Step 1: Writing the C Program to Log Messages

- I started by creating a C program that logs various messages with different severity levels using the `syslog()` function. 

- I ran this program to generate logs with different severities that will be forwarded to the syslog server.

### Step 2: Configuring `rsyslog` on the Client Machine to Forward Logs

To forward logs with severity greater than `warning` to a remote syslog server, I configured `rsyslog` on the client system.

#### Configuration:
I edited the `rsyslog` configuration file (`/etc/rsyslog.conf`) and added the following line:

```rsyslog.conf
if $syslogseverity-text < 'warning' then @10.10.1.33:514
```
#### Explanation:
$syslogseverity-text < 'warning': This condition checks if the log severity is greater than warning. Severity levels in syslog are ranked as:

    emerg (0) - Emergency: System is unusable.
    alert (1) - Action must be taken immediately.
    crit (2) - Critical conditions.
    err (3) - Error conditions.
    warning (4) - Warning conditions.
    notice (5) - Normal but significant conditions.

- **Condition (`if $syslogseverity-text < 'warning'`)**: This checks if the severity of the log message is greater than `warning` (`err`, `crit`, `alert`, `emerg`).
- **Remote syslog server (`@10.10.1.33:514`)**: The `@` symbol specifies UDP for log forwarding to the IP `10.10.1.33` on port `514`.

### Step 3: Restarting `rsyslog` on the Client Machine

After updating the `rsyslog` configuration, I restarted the `rsyslog` service on the client machine to apply the changes:

```bash
sudo systemctl restart rsyslog
```
### Step 4: Setting Up the Syslog Server on the VM

For the syslog server, I used a VM and configured it to listen for incoming syslog messages.

1. **Install `rsyslog`**: On the VM (acting as the syslog server), I installed `rsyslog` (if it wasn’t already installed).
```bash
sudo apt-get install rsyslog
```
2. **Configure `rsyslog` to Accept Remote Logs**:
   I made sure the VM’s `rsyslog` was configured to accept remote logs. This required ensuring the following lines were present in the `/etc/rsyslog.conf` file (uncommented):
   
   ```rsyslog.conf
   module(load="imudp")  # for UDP support
   input(type="imudp" port="514")  # listening for syslog messages on port 514
   ```

3. **Open Port 514**:
   I made sure that port `514` was open on the VM's firewall to allow incoming syslog traffic:
   
   ```bash
   sudo ufw allow 514/udp
   ```

4. **Restart `rsyslog` on the VM**:
   I restarted `rsyslog` on the syslog server (VM) to apply the changes:
   
   ```bash
   sudo systemctl restart rsyslog
   ```

### Step 5: Verifying Logs on the Syslog Server

Once everything was configured, I ran the C program again on the client machine. Logs with a severity greater than `warning` (i.e., `error`, `critical`) were forwarded to the syslog server.

On the syslog server (VM), I monitored the log files (`/var/log/syslog`) to ensure the logs were being received:

```bash
cat /var/log/syslog
```

