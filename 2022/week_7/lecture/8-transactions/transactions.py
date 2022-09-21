'''
DO NOT RUN: This code is just for demonstrating transaction process in sql, like 2 people pressed like at the same time
'''
from cs50 import SQL

db = SQL(uri)

db.execute("BEGIN TRANSACTION")
rows = db.execute("SELECT likes FROM posts where id = ?", post_id)
likes = rows[0]["likes"]
db.execute("UPDATE posts SET likes = ? WHERE id = ?", likes + 1, post_id)
db.execute("COMMIT")