# Audit Configuration for System Files

## Installation

```bash
sudo apt-get install auditd
sudo systemctl start auditd
```
---

## Audit Rules Configuration
```bash
sudo subl /etc/audit/rules.d/audit.rules
```

### 1. **Auditing `/etc/passwd` Changes**

The `/etc/passwd` file contains user account information, so it is essential to monitor any changes to it.

#### Rule:
```bash
-w /etc/passwd -p wa -k passwd_changes
```

**Explanation:**
- `-w /etc/passwd`: Watches the `/etc/passwd` file for any access or changes.
- `-p wa`: Tracks write (`w`) and attribute changes (`a`) to the file.
- `-k passwd_changes`: Adds a custom key (`passwd_changes`) for easy identification when searching logs.

---

### 2. **Auditing `/etc/sudoers` Changes**

The `/etc/sudoers` file controls user permissions for executing commands as the root user. Changes to this file could indicate unauthorized privilege escalation.

#### Rule:
```bash
-w /etc/sudoers -p wa -k sudoers_changes
```
---

### 3. **Auditing Read Access to `/etc/passwd`**

We want to track read access to the `/etc/passwd` file as this file contains sensitive user information.

#### Rule:
```bash
-w /etc/passwd -p r -k passwd_read
```

**Explanation:**
- `-w /etc/passwd`: Watches the `/etc/passwd` file.
- `-p r`: Tracks read access (`r`) to the file.
- `-k passwd_read`: Adds a custom key (`passwd_read`) for easy identification when searching logs.

---

### 4. **Auditing Read Access to `/etc/shadow`**

The `/etc/shadow` file contains encrypted password information, which makes it a critical file to monitor for any unauthorized access.

#### Rule:
```bash
-w /etc/shadow -p r -k shadow_read
```
---

## Applying the Audit Rules

After writing the rules in the `/etc/audit/rules.d/` directory, we can apply them using the following command:

```bash
sudo systemctl restart auditd
```

- After adding the rules, verify them with:

```bash
sudo auditctl -l
```
- Open shadow file to test auditing and then search for rule related audit
```bash
sudo cat /etc/shadow
sudo ausearch -k shadow_read
```
---

# Syslog and Log Forwarding Exercise

### 1. Installed Sysmon on Windows VM

- **Sysmon** (System Monitor) was installed on a Windows VM to log detailed events like process creation, network connections, and file access.
- Downloaded Sysmon from [Microsoft Sysinternals](https://docs.microsoft.com/en-us/sysinternals/downloads/sysmon).
- Installed Sysmon with a configuration file to monitor specific events.
  ```bash
  sysmon -accepteula -c sysmon_config.xml
  ```

### 2. Configured EventLog Analyzer on the Windows Machine

- Installed **EventLog Analyzer** on the Windows machine (the central log server) to collect logs from various sources, including Sysmon and Kali Linux.
- Configured EventLog Analyzer to receive and analyze logs.

### 3. Installed and Configured NXLog to Forward Sysmon Logs to Syslog Server

- Installed **NXLog** on the Windows VM to forward Sysmon logs to the Syslog server (EventLog Analyzer).
- Modified the `nxlog.conf` configuration file to collect logs from the `Microsoft-Windows-Sysmon/Operational` log and forward them to the Syslog server.

- Restarted NXLog to apply the configuration:
  ```bash
  net stop nxlog
  net start nxlog
  ```

### 4. Installed and Configured Auditd on Kali Linux VM

- Installed **auditd** on the Kali Linux VM to monitor system activities such as file accesses and changes to critical system files.
  ```bash
  sudo apt-get install auditd
  ```

- Created custom audit rules to monitor specific activities:
  ```bash
  -w /etc/passwd -p wa -k passwd_changes
  -w /etc/sudoers -p wa -k sudoers_changes
  ```

- Restarted the auditd service:
  ```bash
  sudo systemctl restart auditd
  ```

### 5. Forwarded Auditd Logs from Kali Linux to Syslog Server

- Installed **rsyslog** on the Kali Linux VM to forward auditd logs to the Syslog server.
  ```bash
  sudo apt-get install rsyslog
  ```

- Edited the `rsyslog.conf` file to forward logs to the Syslog server. Added the following rule to send all logs to the Syslog server at `192.168.1.100`:
  ```bash
  *. @192.168.1.100
  ```
  This rule specifies that all logs (denoted by `*.*`) should be forwarded to the Syslog server with the IP `192.168.1.100` over UDP.

- Restarted the rsyslog service to apply the changes:
  ```bash
  sudo systemctl restart rsyslog
  ```

