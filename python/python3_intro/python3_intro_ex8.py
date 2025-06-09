import math

def is_prime(num):
    if num != int(num):
        return False  # floats aren't prime
    if num <= 1:  # negative numbers and 1 aren't prime
        return False
    if num <= 3:  # 2 or 3
        return True
    i = 2
    while i <= math.sqrt(num):  # If a number has a divisor larger than its sqrt, the corresponding pair divisor must be smaller than sqrt
        if num % i == 0:
            return False
        i += 1
    return True

number = float(input("Enter a number: "))  # accepts any number
print("Is the number prime? ", is_prime(number))
