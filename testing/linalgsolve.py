import numpy as np

a = np.array([[0, 0, 0], [0, 18, -15], [0, -15, 25]])
b = np.array([0, 54, -120])

x = np.linalg.solve(a, b)

print(x)
