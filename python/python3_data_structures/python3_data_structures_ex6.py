def remove_numbers(lst):
    while len(lst) > 0:
        if len(lst) == 1:
            print(lst.pop(0))
        else:
            print(lst.pop(1))

my_list = [1, 2, 3, 4, 5, 6]
remove_numbers(my_list)
