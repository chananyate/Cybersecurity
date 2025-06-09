class PasswordTooShortError(Exception):
    pass

class WeakPasswordError(Exception):
    pass

count = 0

def pass_letter(password):
    global count
    Flag = 0
    flag = 0
    for ele in password:
        if ele.isupper():
            Flag = 1
        elif ele.islower():
            flag = 1
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

def validate_password(password):
    if len(password) < 8:
        raise PasswordTooShortError("Password must be at least 8 characters long")

    pass_letter(password)
    pass_num(password)
    pass_char(password)

    if count == 0:
        raise WeakPasswordError("Password is weak")
    elif count == 1:
        return "fair"
    elif count == 2:
        return "strong"
    else:
        return "very strong"

try:
    password = input("Enter password: ")
    validate_password(password)
    print(f"Password strength: {'fair' if count == 1 else 'strong' if count == 2 else 'very strong'}")
except PasswordTooShortError as e:
    print(f"Error: {e}")
except WeakPasswordError as e:
    print(f"Error: {e}")
except Exception as e:
    print(f"Unexpected error: {e}")
