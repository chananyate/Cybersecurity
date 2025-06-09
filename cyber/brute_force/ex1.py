"""
Brute Force Hash Cracker

This script attempts to crack hashed passwords using a brute-force approach.  
It systematically generates all possible passwords from a given character set and length,  
computes their hash using both SHA-256 and SHA-1, and compares them to the target hashes.
If a match is found for either hash, the script prints the cracked password and stops.

"""


import itertools
import hashlib
import time
import string

def generate_candidates(charset, length):
    # Generates all possible passwords of the given length using characters from charset.
    # itertools.product(charset, repeat=length) creates tuples of length 'length' with all possible combinations.
    # ''.join(candidate) converts each tuple into a string (like ('a', 'b', 'c') → "abc").
    return (''.join(candidate) for candidate in itertools.product(charset, repeat=length))

def hash_function(password, algorithm="sha256"):
    """Generate hash of a password using SHA-256 or SHA-1."""
    if algorithm == "sha256":
        return hashlib.sha256(password.encode()).hexdigest()
    elif algorithm == "sha1":
        return hashlib.sha1(password.encode()).hexdigest()
    else:
        raise ValueError("Unsupported algorithm")

def brute_force_attack(target_hash, charset, length, algorithm="sha256"):
    """Perform brute-force attack and measure time per attempt.
       The default value algorithm="sha256" ensures that if no algorithm is specified, the function defaults to using SHA-256."""
    attempts = 0
    total_time = 0
    start_time = time.time()
    
    for candidate in generate_candidates(charset, length):
        attempts += 1
        attempt_start = time.time()
        
        if hash_function(candidate, algorithm) == target_hash:
            attempt_end = time.time()
            print(f"Password cracked: {candidate}")
            print(f"Total time: {attempt_end - start_time:.4f} seconds")
            print(f"Total attempts: {attempts}")
            print(f"Average time per attempt: {(attempt_end - start_time) / attempts:.6f} seconds")
            return candidate, attempts, attempt_end - start_time
        
        attempt_end = time.time()
        total_time += (attempt_end - attempt_start)
    
    print("Password not found (unexpected case).")
    return None, attempts, total_time

def run_tests():
    """Run brute-force attack on different password compositions and measure time."""
    test_cases = [
        ("test", string.ascii_lowercase, 4),
        ("hello", string.ascii_lowercase, 5),
        ("538201", string.digits, 6),
        ("5913756", string.digits, 7),
        ("36281293", string.digits, 8),
        ("battle", string.ascii_lowercase, 6),
        ("grx89", string.ascii_lowercase + string.digits, 5),
    ]

    algorithms = ["sha256", "sha1"]

    for password, charset, length in test_cases:
        for algorithm in algorithms:
            print(f"\nTesting {algorithm.upper()} with password '{password}' (Length: {length})")
            target_hash = hash_function(password, algorithm)
            brute_force_attack(target_hash, charset, length, algorithm)

if __name__ == "__main__":
    run_tests()
