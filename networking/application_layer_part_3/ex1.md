## Setting up the ssh with key

# Once the VM is running, we ensure that SSH is enabled. 

'''bash
sudo systemctl enable ssh
sudo systemctl start ssh

# We connect to the VM using the password

'''bash
ssh username@VM_IP

# We generate an SSH key on our host machine 

'''bash
ssh-keygen

# We copy the public key to the VM

'''bash
ssh-copy-id username@VM_IP

# We SSH using the key

'''bash
ssh -i path_to_private_key username@VM_IP

# We disable Password Authentication by editing the SSH configuration on the VM

'''bash
sudo nano /etc/ssh/sshd_config

PasswordAuthentication no
PubkeyAuthentication yes
AuthorizedKeysFile .ssh/authorized_keys .ssh/authorized_keys2

# We restart ssh

'''bash
sudo systemctl restart ssh

# To manage SSH keys for sessions, we start the SSH agent on our local machine:

'''bash
eval "$(ssh-agent -s)"

# We ten add our private key to the ssh agent

'''bash
ssh-add ~/.ssh/id_ed11111

# We can create an SSH configuration file (~/.ssh/config) to specify default options for our VM. This will allow us to connect using just the ssh command with a custom alias.

'''bash 
nano ~/.ssh/config

Host myvm
    HostName 192.168.64.2
    User chananyavm
    IdentityFile ~/.ssh/id_ed25519

# Now to automatically use the provided options (IP address, user, private key) without needing to type them every time we can simply run:

'''bash
ssh myvm

# We create a file on the vm

'''bash
echo "Well hello there" > testfile.txt


# We then copy the file from the vm to the host machine. On the host machine:

'''bash
scp username@VM_IP:/path/to/testfile.txt /path/to/destination/


## Difference bewteen SFTP (secure file transfer protocol) and SCP (secure copy protocol)

SFTP and SCP are both methods for transferring files over a secure SSH connection, but they have key differences. SFTP provides an interactive file transfer interface, allowing us to navigate directories, list files, and perform various file operations. It's more versatile and works similarly to FTP but over SSH. SCP, on the other hand, is a straightforward command-line tool used for copying files between systems. It's faster for simple transfers but lacks the interactive and flexible features of SFTP.


