def read_lines(n):
    with open('test_file.txt', 'r') as f:
        lines = f.readlines()
        first_n_lines = lines[:n]
        print(first_n_lines)

read_lines(3)