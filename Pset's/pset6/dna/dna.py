import csv
from sys import argv


def main():

    # check for correct number of arguments
    if len(argv) != 3:
        exit("Usage: python dna.py data.csv sequence.txt")
        
    # open csv file
    datafile = open(argv[1], "r")
    database = csv.DictReader(datafile)
    
    # open dna text file    
    with open(argv[2], "r") as seqfile:
        sequence = seqfile.read()
    
    # def dict "counts" 
    counts = {}
    
    # getting counts of strands after column "name"  
    for strs in database.fieldnames[1:]:
        counts[strs] = match(sequence, strs)
    
    # checking each row in database for matching profile, and printing result
    for row in database:
        if all(counts[strs] == int(row[strs]) for strs in counts):
            print(row["name"])
            return
    print("No match")
    
    # closing database file
    database.close()
    
    
# defining a function to get a longest sequence of matching strands in given dna file    
def match(sequence, strs):
    longest = 0
    length = len(strs)
    
    # for loop to iterrate through dna sequence file
    for i in range(len(sequence)):
        count = 0
        while True:
            start = i + length * count
            end = start + length
            
            # if found the match, increment count
            if sequence[start:end] == strs:
                count += 1
            else:
                break
            
        # updating longest    
        longest = max(longest, count)
    return longest


main()