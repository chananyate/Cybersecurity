def leap_year(year):
    if year % 4 == 0:
        if year % 100 == 0 and year % 400 != 0:
            return False
        else:
            return True
    else:
        return False

year = int(input("Enter year: "))
leap_check = leap_year(year)
print("Is it a leap year? ", leap_check)