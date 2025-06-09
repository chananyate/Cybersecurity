x = 5
def is_global(name):
    return name in globals()  #Checks if the string name is a key in the dictionary returned by globals()

print(is_global('x'))
