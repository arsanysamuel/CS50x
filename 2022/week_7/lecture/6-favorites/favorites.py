import csv
from cs50 import SQL

db = SQL("sqlite:///favorites.db")  # database uri

title = input("Title: ").strip()

# Executing SQL command inside python, returning list of dict rows, setting the key of dicts to counter
# AS: aliasing keyword for columns and tables etc
rows = db.execute("SELECT COUNT(*) AS counter FROM favorites WHERE title LIKE ?", title)

print(rows[0]["counter"])
