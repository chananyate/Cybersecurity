1. User Creation and Home Directory Setup

Part 1: Use two commands to create labuser1 and set a password.

sudo useradd labuser1
sudo passwd labuser1

sudo usermod -s /bin/bash labuser1
sudo cp /etc/skel/.bashrc /home/labuser1/
sudo cp /etc/skel/.profile /home/labuser1/
sudo chown labuser1:labuser1 /home/labuser1/.bashrc /home/labuser1/.profile
su - labuser1

Part 2: Use a single command to create labuser2 with a home directory labusers_home.

   sudo useradd -m -d /home/labusers_home labuser2
   sudo passwd labuser2

2. File Creation and Permission Modification

Step 1: Create a file and check default permissions.

touch myfile
ls -l myfile

Default permissions are usually rw-r--r-- for a regular file (depending on your umask).

Step 2a: Modify permissions so the owner can read/write, others can only execute. Here are two ways:

Method 1: using chmod numbers:

chmod 611 myfile

Method 2 (using symbolic modes):

chmod u+rw,go=x myfile

Step 2b: Change the default permission set by adjusting the umask.

umask 027 (this prevents group and others from having write access by default)

3. Script Creation for Active Sockets

Write a script activesockets to display listening sockets and their owning programs:

#!/bin/bash
sudo lsof -i -P -n | grep LISTEN

Make it executable:

chmod +x activesockets

4. Making Script Globally Executable

Move the script to a directory in PATH - /usr/local/bin:

sudo mv activesockets /usr/local/bin/

Now, any user can run the activesockets script from anywhere by just typing activesockets.

5. Directory Permissions and Copying

Step 1: We create a directory with specific permissions:

mkdir mydirectory
chmod 777 mydirectory (rwxrwxrwx)

drwxrwxrwx 2 chananyatempleman chananyatempleman 4096 Oct 28 19:05 mydirectory/

Step 2: We copy it to another location and observe permissions:

cp -r mydir mydirectory_copy
ls -ld mydirectory_copy

drwxr-x--- 2 chananyatempleman chananyatempleman 4096 Oct 28 19:09 mydirectory_copy/

The permissions might change if the parent directory’s default permissions apply. To preserve permissions, we need to use the -p option:

cp -rp mydir mydirectory_copy

ls -ld mydirectory_copy2
drwxrwxrwx 2 chananyatempleman chananyatempleman 4096 Oct 28 19:05 mydirectory_copy2

6. File Permissions for Root and User Access

Step 1: As root, we create a file with no permissions for other and group:

sudo touch restricted_file
sudo chmod 600 restricted_file

Step 2: Switch to a normal user and try to access the file:

chananyatempleman@localhost:~/git/system_programming$ cat restricted_file  (This should result in a "Permission denied" error)

cat: restricted_file: Permission denied

7. Changing file owner without sudo

We switch to root and create a file:

sudo touch owned_by_root

Then we make a copy of this file 

cp owned_by_root

8. Hypothesis Test for File and Directory Removal

Step 1: We create a folder test and navigate into it:

mkdir test
cd test

Step 2: As root, we create root_folder, root_rec_folder, and root_file:

sudo mkdir root_folder root_rec_folder
sudo touch root_file
sudo touch root_rec_folder/root_rec_file

Step 3: We switch back to a normal user and check permissions:

ls -l
ls -l root_rec_folder

Prediction and Test:

If permissions do not allow deletion by the normal user, attempts to delete these files will result in permission errors.

Example:

chananyatempleman@localhost:~/test$ rm root_file  (Should fail due to lack of permission)

rm: remove write-protected regular empty file 'root_file'? 

chananyatempleman@localhost:~/test/root_rec_folder$ rm root_rec_file 
rm: remove write-protected regular empty file 'root_rec_file'? y
rm: cannot remove 'root_rec_file': Permission denied


Explanation: The difference is the first one wasn't creted using sudo so it could be deleted without.