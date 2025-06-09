def str_to_dict(str):
    my_dict = {}
    for char in str:
        my_dict[char] = str.count(char)
    return my_dict

string = "banana"
output = str_to_dict(string)
print(output)
