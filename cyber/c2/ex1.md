

## a. Check how many users are registered on this machine
To check how many users are registered on the system, I used the following command:
```bash
wc -l /etc/passwd
```
This command returned the total number of lines in the `/etc/passwd` file, which corresponds to the number of users.

---

## b. Find their names, and which groups they belong to
To find the usernames and the groups they belong to, I used the following command:
```bash
id <username>
```
---

## c. Find out if each user has root access
To determine if a user has root access, I checked if they were part of the `sudo` group:
```bash
grep 'sudo' /etc/group
```
---

## d. Find the shadow file
The shadow file, which contains user passwords and related information, is located at:
```
/etc/shadow
```
I verified its location using:
```bash
ls /etc/shadow
```

---

## e. Read its contents
To read the contents of the shadow file, we used the following command:
```bash
sudo cat /etc/shadow
```
The contents include user names, encrypted passwords, and password expiration information.

---

## f. Take a screenshot of its contents
To take a screenshot of the shadow file contents, I used the `scrot` tool:
   ```bash
   sudo apt install scrot
   scrot shadow_file_screenshot.png
   ```

---

## g. What is this file? What does it contain?
The `/etc/shadow` file is used to securely store user account information, including encrypted passwords. Each line in the file represents a user account, with fields separated by `:`. These fields include:
- **Username**
- **Encrypted password**
- **Last password change date**
- **Password expiration information**


The contents of the shadow file are essential for secure password management and account configuration.

---