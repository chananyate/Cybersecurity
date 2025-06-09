To check DNS resolution
nslookup www.calcalist.co.il

To block outgoing traffic from my computer its ip addresses
sudo iptables -A OUTPUT -d 23.48.109.100 -j DROP
sudo iptables -A OUTPUT -d 23.221.29.196 -j DROP

To verify that rules are in place
sudo iptables -L OUTPUT -v -n