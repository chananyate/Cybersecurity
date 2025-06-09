# Exercise: Automatically Stop EC2 Instances at the End of the Working Day

## Objective
The task was to create a solution to automatically shut down all EC2 instances at the end of the working day and explore an alternative way to stop all EC2 instances automatically.

## Solution 1: Lambda Function and EventBridge Scheduler

### Steps:

1. **Create a Lambda Function to Stop EC2 Instances**:
   - The first step was to create a **Lambda function** that would stop all running EC2 instances. The function was written in Python and used the `boto3` AWS SDK to interact with EC2 services.
   - The function queried all running instances, extracted their instance IDs, and then used the `stop_instances` method to stop them.

2. **Create an IAM Role**:
   - An **IAM Role** was created for the Lambda function with the necessary permissions to interact with EC2 services. The role was given permissions to describe and stop EC2 instances, using policies like `AmazonEC2FullAccess`.
   - The role was attached to the Lambda function to allow it to perform actions on EC2 instances.

3. **Create an EventBridge Rule**:
   - A **scheduled EventBridge rule** was created to trigger the Lambda function automatically at a specified time each day.
   - The rule was configured with a **Cron expression** `0 18 * * ? *`, which runs the Lambda function every day at **18:00 UTC**.
   - The **Lambda function** was selected as the target for this EventBridge rule, ensuring that at 18:00 each day, the Lambda function would be invoked, automatically stopping all EC2 instances.

4. **Permissions for EventBridge**:
   - The Lambda function's **execution role** was updated to allow EventBridge to invoke the Lambda function. This was achieved by ensuring the role had the `lambda:InvokeFunction` permission.
   - This permission was added either manually or automatically through the AWS console during the EventBridge setup.

5. **Testing the Setup**:
   - After everything was configured, the EventBridge rule was tested to ensure that it correctly triggered the Lambda function at 18:00 UTC.
   - The Lambda function logs were checked in **CloudWatch** to confirm that all running EC2 instances were being stopped as expected.

### Outcome:
At the end of the working day (18:00 UTC), all EC2 instances were automatically shut down without any manual intervention.

---

## Solution 2: Using AWS Instance Scheduler

### Steps:

1. **Using AWS Instance Scheduler**:
   - An alternative approach was to use **AWS Instance Scheduler** to automatically stop all EC2 instances at the end of the working day.
   - AWS Instance Scheduler allows you to define start and stop times for EC2 instances using a **schedule**.
   - This schedule can be configured to stop all instances at a specific time, such as 18:00 UTC.
   - This method involves using a combination of AWS **CloudWatch Events** and the **AWS Systems Manager Automation** document to stop instances on a predefined schedule.

2. **Configure Stop Times**:
   - The Instance Scheduler was configured with a stop schedule at the end of the working day, and it was able to automatically shut down all EC2 instances at the specified time.

3. **Monitoring**:
   - The scheduled stop actions were monitored through CloudWatch logs to confirm that EC2 instances were being stopped as expected.

### Outcome:
Using the AWS Instance Scheduler provided a simple and automated way to manage the scheduling of stopping EC2 instances without needing to write custom Lambda functions. This was a more streamlined option for users not wanting to implement a custom script.

---

## Conclusion

Both methods—using **Lambda with EventBridge Scheduler** and **AWS Instance Scheduler**—offer reliable ways to automate the shutdown of EC2 instances at the end of the working day. 

- The **Lambda and EventBridge** solution provides more flexibility, as you can customize the function to add additional logic or actions.
- The **AWS Instance Scheduler** offers a simpler, managed solution for scheduling EC2 instance operations without the need for custom coding.
