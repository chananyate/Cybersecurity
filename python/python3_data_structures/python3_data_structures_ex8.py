def find_max_min(my_dict):
    maximum = max(my_dict, key = my_dict.get)
    print("Key with maximum value is: ", maximum)

    minimum = (min(my_dict, key = my_dict.get))
    print("Key with minimum value is: ", minimum)

dictionary = {'a': 100, 'b': 50, 'c': 101, 'd': 200, 'e': 201}
find_max_min(dictionary)