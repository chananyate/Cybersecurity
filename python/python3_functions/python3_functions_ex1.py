my_list = ["hello", "what", "how", "probably"]
word_removed = "what"
new_words = list(filter(lambda x: x not in word_removed, my_list))
print(new_words)