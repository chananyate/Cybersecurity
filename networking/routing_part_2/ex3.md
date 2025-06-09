## Enabling OSPF on FRR. OSPF allows routers to automatically discover and exchange routing information with each other. Instead of manually configuring static routes, OSPF enables the network to adapt to changes (like new routers) without requiring manual intervention.

## Steps

```bash
sudo apt install frr
sudo subl /etc/frr/daemons
```
# In file
ospfd=yes

```bash
sudo systemctl restart frr
sudo vtysh
```

# In FRRouting CLI

```
configure terminal
router ospf
network 10.0.0.2/30 area 0
network 192.168.56.1/24 area 0
exit
exit
write memory
exit
```
# in VM

```bash
ping 192.168.57.3
```
