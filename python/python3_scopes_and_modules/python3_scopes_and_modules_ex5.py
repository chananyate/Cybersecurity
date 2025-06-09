import sys
import os

def search_file(filename, root_dir):
    for root, dirs, files in os.walk(root_dir): # Traverses the directory tree starting from root_dir
        if filename in files:
            return os.path.join(root, filename) # If found, returns the full path to the file
    return None

def check_file_permissions(file_path):
    change_mode = 0o770  #Octal format
    if os.access(file_path, os.X_OK):  # Returns True if the file has execute permission for at least one of the following: owner, group, others
        print(f"Execute permission is granted for file: {file_path}")
    else:
        print(f"Execute permission is not granted for file: {file_path}")
        print("Changing permission for owner and group")
        os.chmod(file_path, change_mode)
        print(f"New permissions set: {oct(change_mode)}")

args = sys.argv
root_dir = '/'

file_path = search_file(args[1], root_dir)
check_file_permissions(file_path)

