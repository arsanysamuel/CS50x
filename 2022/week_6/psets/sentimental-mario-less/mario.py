# Prints a simple pyramid of a height
h = 0

# Getting user input
while h > 8 or h < 1:
    try:
        h = int(input("Height: "))
    except ValueError:
        pass

# Printing the pyramid
for i in range(1, h + 1):
    print(" " * (h - i) + "#" * i)