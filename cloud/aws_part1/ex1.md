## Overview
This document outlines the steps I followed to launch an Amazon EC2 instance and deploy my Flask-based weather application. The process includes setting up the instance, transferring files, installing dependencies, and running the application.

---

## Steps

### 1. Launching the EC2 Instance
I began by launching an EC2 instance from the AWS Management Console.  
- I selected **Amazon Linux 2** as the AMI and chose **t3.micro** as the instance type (to remain within free-tier eligibility).  
- I configured the instance, ensuring I created or selected an SSH key pair for secure access.  
- After reviewing the configuration, I launched the instance and noted its public IP address for future use.  

---

### 2. Connecting to the Instance
Once the instance was running, I connected to it using SSH using my public ip provided by the instance:  
ssh -i ~/Downloads/FirstInstance-key.pem ec2-user@<16.171.110.131>

I ensured the key file had proper permissions by running:
chmod 400 ~/Downloads/FirstInstance-key.pem

3. I attached a new General Purpose SSD (gp2) EBS volume to the instance:

I created the volume with a size of 1 GiB in the same Availability Zone as the instance.
In the AWS Management Console, I navigated to the Volumes section and attached the volume to the instance.
After attaching, I logged into the instance and verified the new volume was detected:

lsblk
sudo mkdir /data
sudo fdisk /dev/nvme1n1
sudo mkfs -t xfs /dev/nvme1n1p1
sudo mount /dev/nvme1n1p1 /data

4. Taking a Snapshot
I created a snapshot of the instance's EBS volume to save its current state:

I navigated to the Volumes section in the AWS Management Console, selected the volume, and clicked Create Snapshot.
I provided a description for the snapshot and confirmed its creation.

5. Creating a Launch Template
I created a launch template to replicate the instance's configuration:

I navigated to the Launch Templates section in the AWS Management Console and selected Create Launch Template.
I provided details such as the AMI, instance type, security groups, and storage settings.

6. Creating an AMI
I created an Amazon Machine Image (AMI) from the running instance:

In the AWS Management Console, I navigated to the Instances section and selected the instance.
I clicked Actions > Image and templates > Create Image.
I provided a name and description for the AMI and confirmed the operation.

7. Deploying the Python Weather Application

6. Configuring the Instance
After connecting to the instance, I configured it for my application.

sudo yum update -y
sudo yum install python3
sudo yum install python3-pip

I transferred my project files (app.py, requirements.txt, index.html) using scp:

scp -i ~/Downloads/FirstInstance-key.pem -r ~/git/python/python3_weather_app/ ec2-user@<PUBLIC_IP>:/home/ec2-user/

I updated app.py to allow external access:

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)


In my ex2 instance, I configured the security group to allow HTTP traffic on port 5000.

I started the Flask application:

python3 app.py

I accessed the app in a browser at http://<PUBLIC_IP>:5000.