# importing function from cs50 lib
from cs50 import get_string

# Promping user to gives us text
text = get_string("Text: ")

# setting base value
letters = 0
words = 1
sentences = 0

# making for-loop to calculate end values
for i in range(len(text)):
    if text[i].isalpha():
        letters += 1
    if text[i].isspace():
        words += 1
    if (text[i] == '!' or text[i] == '?' or text[i] == '.'):
        sentences += 1
    
# calculating index per given formula using our collected data above
L = (letters / words) * 100
S = (sentences / words) * 100
index = round(0.0588 * L - 0.296 * S - 15.8)

# printing rsults
if (index <= 0):
    print("Before Grade 1")
elif (index >= 16):
    print("Grade 16+")
else:
    print(f"Grade {index}")

