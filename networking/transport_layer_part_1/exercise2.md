TCP - My computer sent an HTTP request to my home router (port 80), which will then send it to the server.
This frame includes sequnce numbers and acknowledgment numbers and a window size of only 256 bytes, and a header length of 32 bytes, which includes timestamps.

UDP - My TV is sending a frame to the broadcast addrerss ip (with MAC addres ff:ff:ff:ff:ff:ff). It's broadcasting its presence to allow other devices (like smartphones) to discover it on the network. It's using UDP for its low latency, no retransmission and overall better performance than TCP, which is crucial for live streaming. The UDP frame has no sequence and acknowledgment numbers, and has a smaller header of 20 bytes.

TLSv1.2 - a security protocol who's purpose is to provide secure communication by encrypting the data exchanged between applications and ensuring its integrity and authenticity. This is sent when my application wants to send data to the server so it needs to be encrypted first, and then handed off to the TCP layer.

TCP keepalive requests - command in filer box - tcp && !(tcp.payload) - thisindicates a packet with no payload, just the ACK flag set, while all other flags are not set, since no connection is being established, only a confirmation hat the conection is still open.
