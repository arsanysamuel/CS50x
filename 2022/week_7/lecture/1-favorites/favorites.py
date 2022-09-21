import csv

favorites = {}

with open("favorites.csv", "r") as file:
    reader = csv.DictReader(file)  # reading csv file to a dictionary
    for row in reader:
        title = row["title"].strip().upper()  # strip from spaces and convert to uppercase
        if not title in favorites:  # initialize if not in favorites
            favorites[title] = 0
        favorites[title] += 1  # increment the value

# Printing favorites sorted by most popular, lambda function for the key is returning value of key title
for title in sorted(favorites, reverse=True, key=lambda title: favorites[title]):
    print(title, favorites[title])