# importing function from cs50 lib
from cs50 import get_float

dollars = 0

# condition to keep number positive
while dollars <= 0:
    # promts user to give number of change
    dollars = get_float("Change owed: ")
    
# convertion of dollars in cents
cents = round(dollars * 100)

# coins count
coins = 0

# deduction of cents to determine coins needed for change
while cents >= 25:
    cents -= 25
    coins += 1    
    
while cents >= 10:
    cents -= 10
    coins += 1 
    
while cents >= 5:
    cents -= 5
    coins += 1     
    
while cents >= 1:
    cents -= 1
    coins += 1 
    
# printing the result
print(coins)