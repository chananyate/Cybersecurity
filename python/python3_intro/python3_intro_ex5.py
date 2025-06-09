from sys import flags

count = 0

def pass_letter(password):
    global count
    Flag = 0
    flag = 0
    for ele in password:
        if ele.isupper():
            Flag = 1
        elif ele.islower():
            flag = 1;
        if Flag and flag:
            count += 1
            break

def pass_num(password):
    global count
    for ele in password:
        if ele.isdigit():
            count += 1
            break

def pass_char(password):
    global count
    char_set = {'@', '#', '%', '&'}
    if any(c in password for c in char_set):
        count += 1

password = input("Enter password: ")
if len(password) < 8:
    print("password must be at least 8 characters long")
    exit()

pass_letter(password)
pass_num(password)
pass_char(password)

if count == 0:
    print("weak")
elif count == 1:
    print("fair")
elif count == 2:
    print("strong")
else:
    print("very strong")