from collections import Counter

def dict_unique(dict):
    my_list = []
    value_counts = Counter(dict.values())  #since count() is designed for ordered containers it cannot be used on dicts
    for value, count in value_counts.items():
        if count == 1:
            my_list.append(value)
    return my_list

my_dict = {'a': 1, 'b': 2, 'c': 1, 'd': 3}
output = dict_unique(my_dict)
print(output)
