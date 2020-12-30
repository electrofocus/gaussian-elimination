import numpy as np

n = int(input("Type matrix size: "))

A = np.random.rand(n, n) * 50
X = np.random.rand(n) * 50

b = A @ X

with open("input.txt", "w") as file_1:
    np.savetxt(file_1, [n], fmt="%d")
    np.savetxt(file_1, A, fmt="%.2f")
    np.savetxt(file_1, b, fmt="%.2f")

with open("X.txt", "w") as file_2:
    np.savetxt(file_2, X, fmt="%.2f")

print("You can find your data in 'input.txt' and 'X.txt' files")
