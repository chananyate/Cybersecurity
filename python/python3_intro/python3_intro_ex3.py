def temp_conv(celsius):
    return (celsius*(9/5) + 32)

cel_temp = float(input("Enter degrees in celsius: "))

fahr_temp = temp_conv(cel_temp)
print("In fahrenheit: ", fahr_temp)