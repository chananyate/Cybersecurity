def reverse(num):
   return int(str(num)[::-1])

number = int(input("Enter a number: "))
flipped = reverse(number)
print("The reversed number is: ", flipped)