## ICMP Redirect is a message sent by routers to inform hosts that a better route exists for a destination, effectively advising the host to update its routing table.

## Risks:
ICMP redirects can be abused for attacks like Man-in-the-Middle (MITM). If a malicious device intercepts the ICMP redirect message, it can redirect traffic through itself, intercepting sensitive data.

## A router can be configured to send ICMP redirect messages, but security best practices usually suggest disabling them to avoid these risks.

To enable ICMP redirects:
```bash
sudo sysctl -w net.ipv4.conf.all.accept_redirects=1
sudo sysctl -w net.ipv4.conf.default.accept_redirects=1
```
## Guest Response to ICMP Redirect: Guests ususally accept and act upon ICMP redirect messages by modifying their routing tables, although this behavior can be disabled in the guest's network settings or in sysctl configuration.
