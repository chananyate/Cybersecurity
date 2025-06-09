# AWS EC2 Bastion Host and Private Instance Setup

## Objective
The goal of this exercise was to set up a bastion host in a public subnet and a private EC2 instance in a private subnet, ensuring secure access to the private instance via the bastion host.

## Steps Completed

### 1. Initial Setup
- I started with an existing EC2 instance (`i-0e9de2b5746c1b6bb`) in a public subnet.
- I identified that this instance needed to be moved to a private subnet to follow best practices for security.

### 2. Created a Bastion Host
- I launched a new EC2 instance (`i-044111dfb070eb414`) in a public subnet (`subnet-0d21824fde6e4f2e1`).
- While configuring the instance, I ensured it had the following:
- **Public Subnet**: A subnet with a route to the Internet Gateway (IGW).
- **Auto-assign Public IPv4 Address**: Enabled.
- **Security Group**: Allowed inbound SSH traffic (port 22) from `0.0.0.0/0`.

### 3. Verified the Public Subnet Configuration
- I checked the route table associated with the public subnet to ensure it had a route to the Internet Gateway.
- I confirmed that the public subnet allowed public IP assignment.

### 4. Updated the Private Instance Subnet
- I used the AWS CLI to stop my original EC2 instance (`i-0e9de2b5746c1b6bb`).
- I moved the instance to a private subnet (`subnet-022fc99d03d61fd15`) using the CLI:
aws ec2 modify-instance-attribute --instance-id i-0e9de2b5746c1b6bb --subnet-id subnet-022fc99d03d61fd15 --region us-east-1
- I started the instance again using:
aws ec2 start-instances --instance-ids i-0e9de2b5746c1b6bb --region us-east-1

### 5. Connected to the Private Instance via Bastion Host
- Since my private key was on my local machine, I launched the bastion host with the flag -A to enable **SSH Agent Forwarding** to access the private instance:

ssh -i /path/to/your/key.pem -A ec2-user@<bastion-public-ip>
ssh ec2-user@<private-instance-private-ip>

### 6. Verified Configuration
- I ensured the bastion host was in the public subnet and the private instance was securely placed in the private subnet.
- I confirmed that the private instance could not be accessed directly from the internet, only through the bastion host.