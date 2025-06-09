def remove_from_list(list):
    list_copy = list.copy()
    i = 0
    while i < len(list_copy):
        if type(list_copy[i]) != str:  #using a copy ensures the list is not modified throughout the iterations
            list.remove(list_copy[i])
        i += 1

list = [1, "apple", 3, 5, "banana", 7]
remove_from_list(list)
print(list)