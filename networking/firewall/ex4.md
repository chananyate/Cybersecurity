# Exercise 4: Allow Only One SSH Connection in Parallel

In this exercise, we configure `iptables` to limit the number of concurrent SSH connections to a single connection at any given time.

## Steps

### 1. **Command to Allow Only One SSH Connection in Parallel**

```bash
sudo iptables -A INPUT -p tcp --dport 22 -m connlimit --connlimit-above 1 --connlimit-mask 32 -j REJECT
```

### 2. **Testing the Rule**

1. Opened an SSH connection to the server from a friend's machine:
   ```bash
   ssh chananyatempleman@10.10.0.194
   ```
2. Attempt to open a second SSH connection from a separate tab in the same machine:
   ```bash
   ssh chananyatempleman@10.10.0.194
   ```
   - The second connection was rejected.

### 3. **Verifyied the Rule**
To view the active `iptables` rules:
```bash
sudo iptables -L --line-numbers
```

### 4. **Remove the Rule When Done**

```bash
sudo iptables -D INPUT -p tcp --dport 22 -m connlimit --connlimit-above 1 --connlimit-mask 32 -j REJECT
```
