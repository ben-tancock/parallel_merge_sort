import random
import sys

n = int(sys.argv[1])

x = []

f = open('input.txt', 'w')
f.write(str(n) + '\n')
for i in range(n):
    y = random.randint(1, 100000)
    x.append(y)
    f.write(str(y) + '\n')
f.close()

f = open('output.txt', 'w')
f.write(str(n) + '\n')
x.sort()
for y in x:
    f.write(str(y) + '\n')
f.close()
