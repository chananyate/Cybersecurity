def element_in_lists(list1, list2):
    my_list = []
    for element in list1:
        if element in list1 and element in list2:
            my_list.append(element)
    return my_list

list1 = [1, 3, "apple", 7]
list2 = ["apple", "banana", 7, 4]

output = element_in_lists(list1, list2)
print(output)


