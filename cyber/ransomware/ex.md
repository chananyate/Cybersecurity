
# AES-256 Encryption and Decryption Exercise

This exercise demonstrates how ransomware encrypts and decrypts files using AES-256 encryption, while securely handling the encryption key. 

## Create a Folder and Copy Files

Create a directory and copy some files into it.

```bash
mkdir encrypted_folder
cp /path/to/your/files/* encrypted_folder/
```

## Encrypt the Folder's Contents Using AES-256

Now, we'll encrypt all files inside `encrypted_folder` using AES-256 with a randomly generated key.

### Generate a Secure AES-256 Key

```bash
openssl rand -hex 32 > /home/chananya/Downloads/aes_key.txt
```

### Encrypt Files Using AES-256

We’ll use OpenSSL to encrypt each file inside `encrypted_folder`.

```bash
for file in encrypted_folder/*; do
    openssl enc -aes-256-cbc -salt -in "$file" -out "$file.enc" -pass file:/home/chananya/Downloads/aes_key.txt
    rm "$file"  # Remove original file
done
```

### Securely Send the AES Key to Another Location

To simulate **exfiltration of the key**, let's move the AES key to a separate directory (mimicking an attacker's server).

```bash
mv aes_key.txt ~/safe_location/  # Mimicking exfiltration
```

### Encrypt the AES Key Using RSA for Exfiltration

If simulating ransomware, the AES key would be encrypted using an attacker’s public RSA key before being exfiltrated.
By encrypting the AES key with the RSA public key, it ensures that even if the key is intercepted, it can't be read without the corresponding private key.

```bash
openssl rsautl -encrypt -pubin -inkey attacker_public.pem -in aes_key.txt -out aes_key_encrypted.bin
```
openssl rsautl \                  # Uses OpenSSL's RSA utility for encryption/decryption
    -encrypt \                     # Specifies that we want to encrypt data
    -pubin \                        # Indicates that we are using a public key for encryption
    -inkey attacker_public.pem \    # Specifies the public key file (attacker's public key)
    -in aes_key.txt \               # The input file containing the AES-256 key to be encrypted
    -out aes_key_encrypted.bin      # The output file where the encrypted AES key will be stored


## Look at the Encrypted Files

Lets try opening an encrypted file. It should appear as **random bytes**.

```bash
cat bst.c.enc
```
## Decrypt the AES Key and Use It to Decrypt the Files

Before we can decrypt the files, we must first decrypt the AES key.

### Decrypt the AES Key Using RSA

If the AES key was encrypted using RSA, lets use private RSA key to decrypt it.
```bash
openssl rsautl -decrypt -inkey private.pem -in aes_key_encrypted.bin -out aes_key.txt
```
### Decrypt Files Using the Decrypted AES Key

Once we have the decrypted AES key, we can now decrypt the files.

```bash
for file in encrypted_folder/*.enc; do
    openssl enc -aes-256-cbc -d -in "$file" -out "${file%.enc}" -pass file:/home/chananya/Downloads/aes_key.txt
    rm "$file"  # Remove encrypted file
done
```

## Compare the Decrypted and Original Files

To verify that decryption worked correctly, we compare the decrypted files with the original ones.

```bash
diff encrypted_folder/ /path/to/original_files/
```

If there’s **no difference** (no output), the encryption and decryption process worked great.

---

## Post-Exercise Questions

### **1. How did you encrypt your generated secure key? Why?**

We encrypted the AES key using **RSA** because:
- The AES key is sensitive and needs protection before it is transmitted or exfiltrated.
- RSA encryption ensures that only the intended recipient (with the private RSA key) can decrypt the AES key.

### **2. How did you generate a secure key? What does a secure key mean?**

The secure key was generated using a **cryptographically secure random number generator (CSPRNG)**. A secure key:
- Is **random** and **non-repetitive** (hard to guess).
- Has enough **entropy** to prevent brute-force attacks (AES-256 uses a 256-bit key).
- Is **stored securely** (not in plaintext or accessible to unauthorized parties).

### **3. Why did we choose AES-256?**

AES-256 was chosen because:
- **High security** – AES-256 provides a high level of security and is resistant to brute-force attacks.
- **Efficiency** – AES is designed to be fast for encrypting large volumes of data.
- **Industry standard** – AES-256 is widely used and trusted by governments and organizations worldwide.

### **4. Why did we not choose RSA?**

RSA is not ideal for encrypting large files because:
- **Slower performance** – RSA encryption is much slower than AES when encrypting large volumes of data.
- **Limited to small data** – RSA is typically used for encrypting small amounts of data, such as encryption keys, rather than entire files.

### **5. Why would we generate a new key for every infectee? Why not use the same one?**

Generating a new key for each victim increases security:
- **Avoids mass decryption** – If one key is compromised, only one victim's files are at risk.
- **Stronger encryption** – A unique key ensures that each victim’s data is securely encrypted with a different key.
- **Ransomware tactic** – Attackers usually provide a unique key for each victim to ensure they are forced to pay the ransom.

