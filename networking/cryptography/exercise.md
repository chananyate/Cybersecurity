a. GPG (GNU Privacy Guard) is a free and open-source software that allows you to encrypt and sign data and communications.

b. Encryption vs signatures - The purpose of encryption is confidentiality. It takes plaintext and converts it into ciphertext to protect the content from being understood by unauthorized parties.
The purpose of signatures, on the other hand, is authenticity and integrity. Signing a message is the process of creating a digital signature using the sender's private key. This signature can be verified by anyone with the sender's public key to ensure that the message was indeed sent by the claimed sender and that it has not been tampered with.

c. Typed command - 
gpg --full-generate-key
then selected keysize, key type, expiration date, user information, set a passphrase. 
verified the key was generated using this command - 
gpg --list-keys

d. cretaing the file and making copies -
echo "Hello world." > confidential_exercise.txt

for i in {1..3}; do cp confidential_exercise.txt "confidential__exercise_copy$i.txt"; done

encrypting the files with my own public key -
for file in confidential_exercise*.txt; do gpg --encrypt --recipient chananya.te@gmail.com "$file"; done

To sign and compress the files - 
gpg --sign --compress-level 9 confidential_exercise.txt.gpg
To verify - 
gpg --verify confidential_exercise.txt.gpg.sig

Signimg without compression -
gpg --sign secret.txt

Creating a detached signature - 
gpg --detach-sign --output confidential_exercise.txt.sig confidential_exercise.txt
verifying it - 
gpg --verify confidential_exercise.txt.sig confidential_exercise.txt

Encrypting a file I would Uuse when I need to protect the file’s contents from unauthorized access.

Signing a file I would use when I want to confirm the authenticity and integrity of the file.

Creating a detached signature I would use when I need the signature to be separate from the 
file. This can be useful if I need to send the signature and file separately or when I dont't want people to modify the file. 

Compressing, signing, and encrypting a file I would use when I need to secure the file’s contents and confirm its origin in one operation. 
