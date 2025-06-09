def double_every_second_digit(num):
    str_num = str(num)[::-1]  #reverses number and converts it to string
    digits = [int(d) * 2 if i % 2 == 1 else int(d) for i, d in enumerate(str_num)]  #doubles every second digit
    for i, value in enumerate(digits):
        if i % 2 == 1:
            if value > 9:
                digits[i] = value - 9
    sum_of_numbers = sum(digits)
    if sum_of_numbers % 10 == 0:
        print("Valid credit card number")
    else:
        print("Invalid credit card number")
    # return int(''.join(map(str, digits)))  #map converts str casting to each element in digits list. The elements will then be joined into a string with no spaces between them

n = 30569309025903
double_every_second_digit(n)
