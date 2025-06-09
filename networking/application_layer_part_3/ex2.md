## High level:
download the openssh library. Open the auth-passwd.c file and in the auth_password function 
add code to capture the plaintext password. This could be storing it in a file or even sending it over a socket to an attacker-controlled process listening on a specific port. Next, compile the modified sshd and send the executable sshd (ssh daemon) to the VM (the server) and run it. Now whenever the client connects via ssh (port 22) to the VM the modified sshd will both authenticate the user and log or transmit the plaintext password to the configured location.

## Steps

'''bash
git clone https://github.com/openssh/openssh-portable.git

# Modify code in auth_password function

# On local machine

'''bash
./configure
make
sudo make install
scp /Users/chananyatempleman/Documents/openssh-portable/sshd chananyavm@192.168.64.2:/usr/local/sbin

subl ~/.ssh/known_hosts
# Delete or comment out the lines associating the VM's IP with a key

# On VM

'''bash
sudo nano /etc/ssh/sshd_config
in file: set:
PubkeyAuthentication no
PasswordAuthentication yes

'''bash
nano ~/.ssh/authorized_keys
in file:
comment out the line associating a user with a key

'''bash
sudo systemctl stop sshd

# verify nothing is bound to port 22

'''bash
sudo lsof -i :22 

# or

'''bash
sudo ss -tuln | grep ':22'

# Then run the new sshd executable

'''bash
sudo /usr/local/sbin/sshd -D

# Then connect to VM from local machine via ssh

