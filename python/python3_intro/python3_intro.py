def count_char(s, char):
    return s.count(char)

s = input("Enter string: ")
char = input("Lookup the character: ")
result = count_char(s, char)
print("The number of times the character", char, "appears is: ", result)