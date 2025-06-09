Mt ipv6 network is 2a0d:6fc2:68c0:ac00. My global dynamic address is 2a0d:6fc2:68c0:ac00:9371:856a:5be:890e. Was given to me by my ISP (internet service provider).

To configure my ipv6 address I typed the command sudo nmtui and then in the network configuration interface that popped up I navigated to Home1 and then "edit connection" and then changed my ipv6 configuration to manual. Next I entered my own ipv6 address and provided my gateway with command ip -6 route, then I added my DNS server address which I found with command resolvectl status. Then I restarted my network settings with command sudo systemctl restart NetworkManager.

