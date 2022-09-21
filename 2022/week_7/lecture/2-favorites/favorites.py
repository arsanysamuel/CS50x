'''
Using regex to avoid some messing up in the user input data
'''
import csv
import re

counter = 0

with open("favorites.csv", "r") as file:
    reader = csv.DictReader(file)  # reading csv file to a dictionary
    for row in reader:
        title = row["title"].strip().upper()  # strip from spaces and convert to uppercase
        if re.search("^(OFFICE|THE OFFICE)$", title):
            counter += 1

print(f"Number of people who like the office: {counter}")
