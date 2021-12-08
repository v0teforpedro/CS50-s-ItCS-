# importing function from cs50 lib
from cs50 import get_int

# initializing height
height = 0

# prompting user to give us height in proper range
while height > 8 or height < 1:
    height = get_int("Height: ")

# Creating height
for row in range(height):
    # Creating width
    for space in range(height - row - 1):
        # add space (if needed)
        print(" ", end="")
    for column in range(row + 1):
        # add hash
        print("#", end="")
    # printing new line
    print()
