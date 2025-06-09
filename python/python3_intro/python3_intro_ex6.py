def sum_of_divisors(num):
    divisor = 1
    sum = 0
    while divisor <= num:
        if num % divisor == 0:
            sum += divisor
            divisor += 1
        else:
            divisor += 1
    return sum

number = int(input("Enter a number: "))
print("Sum of divisors is: ", sum_of_divisors(number))