def str_rotate(list, n):
    second_part = list[:n:1]
    first_part = list[n::1]
    return first_part + second_part

my_list = [1, 2, "a", 4, 5]
n = 1
rotated_list = str_rotate(my_list, n)
print(rotated_list)