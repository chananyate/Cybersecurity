On computer A set up listening mode on a specific port, preferably in the dynamic range.

Use command nc -l -p 50000 (-l flag for listening mode, -p flag for port number, which acts like a radio frequency, and doesn't have to be 50000 obviously).

On computer B type command nc <computer's_A_ip_address> 50000 to establish a connection.

On computer B type either printf or echo "Text message" | nc <computer's_A_ip_address> 50000

	This whole line will be displayed on computer A immdeiately after pressing enter on computer B.