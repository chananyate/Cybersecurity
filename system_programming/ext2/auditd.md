1. Install auditd using the following commands:

sudo apt update
sudo apt install auditd

2. Start and enable auditd using the following commands:

sudo systemctl start auditd
sudo systemctl enable auditd

3. Add a rule to monitor chmod command calls by editing the audit rules file:

sudo subl /etc/audit/rules.d/audit.rules

Add the following lines (for arm64 processors) in the file to monitor all chmod command calls:

-a always,exit -F arch=aarch64 -S fchmod -k chmod_monitor
-a always,exit -F arch=aarch64 -S fchmodat -k chmod_monitor

This configuration specifies that any call to chmod should be logged. The -k chmod_monitor option assigns a key for easier searching in the logs.

4. Restart auditd to apply the changes:

sudo systemctl restart auditd

5. Create a File and Change Its Mode with chmod

   touch my_test_file.txt

   chmod 755 my_test_file.txt


6. Check the audit log file using the following command:

sudo ausearch -k chmod_monitor