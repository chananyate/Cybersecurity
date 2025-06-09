def dict_to_tuple(my_dict):
    my_tuple = list(my_dict.items())
    return my_tuple

dictionary = {'a': 1, 'b': 2, 'c': 3}
dict_to_tuple_convert = dict_to_tuple(dictionary)
print (dict_to_tuple_convert)