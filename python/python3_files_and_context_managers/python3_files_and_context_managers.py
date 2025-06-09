import os

file_names = ["A.txt", "B.txt", "C.txt", "D.txt", "E.txt", "F.txt", "G.txt", "H.txt", "I.txt",
              "J.txt", "K.txt", "L.txt", "M.txt", "N.txt", "O.txt", "P.txt", "Q.txt", "R.txt"
              "S.txt", "T.txt", "U.txt", "V.txt", "W.txt", "X.txt", "Y.txt", "Z.txt"]
contents = ["A\n", "B\n", "C\n", "D\n", "E\n", "F\n", "G\n", "H\n", "I\n", "J\n", "K\n", "L\n",
            "M\n", "N\n", "O\n", "P\n", "Q\n", "R\n", "S\n", "T\n", "U\n", "V\n", "W\n", "X\n",
            "Y\n", "Z\n"]
for file_name, content in zip(file_names, contents):  # zip allows for iterating over both lists simultaneously, pairing each file_name with its corresponding content.
    with open(file_name, "w") as f:
        f.write(content)