To check current DNS 
nmcli dev show | grep DNS

To change DNS to google's DNS 
nmcli connection modify Home1 ipv4.dns "8.8.8.8 8.8.4.4"

To disable auto configurtion by DHCP
nmcli connection modify Home1 ipv4.ignore-auto-dns yes

To apply the changes made by restarting the connection
nmcli connection down Home1 && nmcli connection up Home1

To check updated DNS
nmcli dev show | grep DNS
