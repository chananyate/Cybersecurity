import smtplib  # For sending emails via SMTP
from email.message import EmailMessage  # For constructing email messages

# Creates an EmailMessage object to construct the email
def send_email_with_attachment(sender, password, receiver, subject, body, attachment_path):
    # Create an EmailMessage object
    msg = EmailMessage()
    msg["From"] = sender
    msg["To"] = receiver
    msg["Subject"] = subject
    msg.set_content(body)  # Email body (plain text)

    """
    Add attachment to email.
    Try is used to handle potential errors gracefully, such as file not found for the attachment 
    or issues during email sending, ensuring the program doesn't crash and provides error messages.
    """
    try:
        with open(attachment_path, "rb") as attachment:  # Opens the file in binary read mode
            file_data = attachment.read()  # Reads the file content
            file_name = attachment_path.split("/")[-1]  # Extracts file name from the path. -1 is the last element of the list resulting from "split", which is the file name

            """
            Adds the file as an attachment to the email. 
            "file_data" contains the binary content of the file. 
            "maintype" specifies the type of content (application for generic fit-all type). 
            "subtype" is "octet-stream," indicating generic binary data.
            """
            msg.add_attachment(file_data, maintype="application", subtype="octet-stream", filename=file_name)

            print("Attached file: ", file_name)
    except FileNotFoundError:
        print(f"Error: The file '{attachment_path}' was not found.")
        return

    # Connects to Gmail SMTP server and sends the email
    try:
        with smtplib.SMTP("smtp.gmail.com", 587) as server:
            server.starttls()  # Starts TLS encryption for security. TLS is used for securing application layer protocols
            print("Connecting to Gmail SMTP server...")
            server.login(sender, password)  
            print("Login successful.")
            server.send_message(msg)  
            print("Email sent successfully!")
    except Exception as e:  # Catches any exceptions during the email-sending process
        print("Error: ", e)

if __name__ == "__main__":
    sender_email = "chananya.te@gmail.com"  
    app_password = "durs xxvt odfe yilf"  
    receiver_email = "chananya.te@gmail.com"    
    email_subject = "Test Email with Attachment"
    email_body = "Well hello there!"
    file_path = "/home/chananyatempleman/git/cyber/networking/application_layer_part_2/word_list_dns_enumaration.txt"  

    send_email_with_attachment(sender_email, app_password, receiver_email, email_subject, email_body, file_path)
