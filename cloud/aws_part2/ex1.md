
## 1. **Create a New EC2 Instance**
- I created a new EC2 instance using the same configuration as the original one (same AMI, instance type, security groups).
- I ensured that the new instance was running and accessible using SSH.

## 2. **Set Up Gunicorn on the New Instance**
- I SSH'd into the new EC2 instance.
- I installed and ran **Gunicorn** to serve the weather app:
  ```bash
  /home/ec2-user/.local/bin/gunicorn --workers 3 --bind 0.0.0.0:5000 app:app
  ```

## 3. **Create a New Target Group**
- I navigated to the **EC2 Console > Target Groups** and clicked **Create target group**.
- I chose the **target type** as **Instance**, set **Protocol** to **HTTP** and **Port** to **5000**.
- I selected the appropriate VPC and clicked **Create**.
- I registered the new EC2 instance as a target in the target group.

## 4. **Create a Load Balancer**
- I went to **EC2 Console > Load Balancers** and clicked **Create Load Balancer**.
- I chose **Application Load Balancer**, set it to **Internet-facing** with **HTTP** protocol on **port 80**.
- I added the **new target group** to the load balancer under **Listeners > Default Action**.
- I selected the Availability Zones and subnets for the load balancer and created it.

## 5. **Test the Setup**
- I checked the **health status** of both EC2 instances in the **Target Group**.
- I accessed the app through the **load balancer’s DNS name** to verify that the traffic was distributed correctly.
```

