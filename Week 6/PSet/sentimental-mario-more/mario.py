from cs50 import get_int

while True:
    n = get_int("Height: ")

    if n > 0 and n <= 8:
        break

for i in range(n):
    tmp1 = i + 1
    tmp2 = n - tmp1
    print(" " * tmp2, end="")
    print("#" * tmp1, end="")
    print("  ", end="")
    print("#" * tmp1)