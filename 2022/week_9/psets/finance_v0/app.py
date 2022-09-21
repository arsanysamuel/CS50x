import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, jsonify
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Getting assets info
    user_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
    assets = db.execute("""SELECT stocks.symbol, stocks.name, assets.shares
            FROM assets LEFT JOIN stocks ON assets.stock_id = stocks.id
            WHERE assets.user_id = ?""",
                        session["user_id"])

    # Getting current stock prices
    total = user_cash
    for i, asset in enumerate(assets):
        quote = lookup(asset["symbol"])
        assets[i]["price"] = quote["price"]
        total += quote["price"] * assets[i]["shares"]

    return render_template("index.html", assets=assets, cash=user_cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        # Getting user input and other info
        symbol = request.form.get("symbol").upper()
        shares = request.form.get("shares")
        quote = lookup(symbol)
        user_balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

        # Ensure correct symbol input
        if symbol == "":
            return apology("Symbol missing")
        elif quote is None:
            return apology("Invalid symbol")

        # Ensure shares is a positive integer
        if shares == "":
            return apology("Shares missing")
        elif not shares.isnumeric() or int(shares) <= 0:
            return apology("Invalid shares value")
        shares = int(shares)

        # Ensure user can afford purchase
        purchase_value = shares * quote["price"]
        if user_balance < purchase_value:
            return apology("Can't afford")

        # Add stock info to stocks table if doesn't exist
        try:
            stock_id = db.execute("INSERT INTO stocks (symbol,name) VALUES (?,?)", symbol, quote["name"])
        except ValueError:
            stock_id = db.execute("SELECT id FROM stocks WHERE symbol = ?", symbol)[0]["id"]
            print(stock_id)

        # Insert the purchase info to the transactions table
        db.execute("INSERT INTO transactions (user_id,stock_id,price,time,shares) VALUES (?,?,?,datetime('now'),?)",
                   session["user_id"], stock_id, quote["price"], shares)

        # Subtract purchase value from user cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", user_balance - purchase_value, session["user_id"])

        # Add shares to user assets, created unique index on user_id and stock_id to avoid duplicates
        try:
            db.execute("INSERT INTO assets (user_id,stock_id,shares) VALUES (?,?,?)",
                       session["user_id"], stock_id, shares)
        except ValueError:
            db.execute("UPDATE assets SET shares = (shares + ?) WHERE user_id = ? AND stock_id = ?",
                       shares, session["user_id"], stock_id)

        flash("Purchase completed")
        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Get transactions data
    transactions = db.execute(""" SELECT stocks.symbol, transactions.shares, transactions.price, transactions.time
        FROM transactions LEFT JOIN stocks ON transactions.stock_id = stocks.id WHERE user_id = ?""",
                              session["user_id"])

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        flash("Logged in.")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/login")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        quote = lookup(symbol)

        if quote:  # If the symbol exists
            return render_template("quoted.html", quote=quote)
        else:
            return apology("Invalid symbol")

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Get user input from the register form post request
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Ensure username was submitted
        if username == "":
            return apology("Username field is blank!")

        # Ensure username doesn't exist
        elif db.execute("SELECT username FROM users WHERE username = ?", username):
            return apology("Username already exists!")

        # Ensure password was submitted
        if password == "":
            return apology("Password field is blank!")
        elif confirmation == "":
            return apology("Password confirmation field is blank!")
        elif password != confirmation:
            return apology("Password and confirmation fields don't match!")

        # If all goes well, store user credentials and login
        db.execute("INSERT INTO users (username,hash) VALUES(?,?)", username, generate_password_hash(password))
        session.clear()
        session["user_id"] = db.execute("SELECT id FROM users WHERE username = ?", username)[0]["id"]
        flash("You've successfully registered.")
        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))

        # Check for symbol input
        symbols = [s["symbol"] for s in db.execute("SELECT symbol FROM stocks")]
        if not symbol or symbol == "":
            return apology("Missing symbol")
        elif symbol not in symbols:
            return apology("Invalid symbol")

        # Check for shares input
        user_shares = db.execute("""SELECT shares FROM assets WHERE user_id = ? AND stock_id = (
            SELECT id FROM stocks WHERE symbol = ?
            )""",
                                 session["user_id"], symbol)[0]["shares"]
        if shares <= 0:
            return apology("Invalid number of shares")
        elif shares > user_shares:
            print(shares)
            print(user_shares)
            return apology("Not enough shares in your assets")

        # If all goes well:
        # Get current share price
        price = lookup(symbol)["price"]

        # Subtract shares from assets
        current_shares = db.execute("""SELECT shares FROM assets WHERE user_id = ? AND stock_id = (
            SELECT id FROM stocks WHERE symbol = ?)""", session["user_id"], symbol)[0]["shares"]

        if current_shares - shares == 0:
            db.execute("""DELETE FROM assets WHERE user_id = ? AND stock_id = (
            SELECT id FROM stocks WHERE symbol = ?)""", session["user_id"], symbol)
        else:
            db.execute("""UPDATE assets SET shares = (shares - ?) WHERE user_id = ? AND stock_id = (
                SELECT id FROM stocks WHERE symbol = ?)""", shares, session["user_id"], symbol)

        # Add cash to user
        db.execute("UPDATE users SET cash = (cash + ?) WHERE id = ?", price * shares, session["user_id"])

        # Add transaction
        db.execute("""INSERT INTO transactions (user_id, shares, price, time, stock_id)
            VALUES (?,?,?,datetime('now'),(
                SELECT id FROM stocks WHERE symbol = ?
                )
            )""",
                   session["user_id"], -shares, price, symbol)

        # Return to index
        flash("Sold shares")
        return redirect("/")

    else:
        assets = db.execute("SELECT stock_id, shares FROM assets WHERE user_id = ?", session["user_id"])
        stocks = db.execute("SELECT symbol FROM stocks WHERE id IN (?)", [n["stock_id"] for n in assets])
        symbols = [n["symbol"] for n in stocks]
        return render_template("sell.html", symbols=symbols)


@app.route("/get-shares")
def get_shares():
    '''Get max share number to set the numeric input max'''

    # Get the symbol from the get request
    symbol = request.args.get("symbol")

    # Get shares number from the database
    shares = db.execute("""SELECT shares FROM assets WHERE stock_id = (
        SELECT id FROM stocks WHERE symbol = ?)""", symbol)[0]["shares"]

    # Return the shares value
    return jsonify({"max": str(shares)})


@app.route("/settings", methods=["GET", "POST"])
def settings():
    """Change account password (just this for now)"""
    if request.method == "POST":

        # Getting form inputs
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Check the old password
        current_hash = db.execute("SELECT hash FROM users WHERE id = ?", session["user_id"])[0]["hash"]
        if not check_password_hash(current_hash, request.form.get("old_password")):
            return apology("Password incorrect")

        # Check new password characters
        invalid_apology = is_password_invalid(password)
        print(invalid_apology)
        if invalid_apology is not None:
            return invalid_apology

        # Check new password
        if password == "":
            return apology("Password field is blank!")
        elif confirmation == "":
            return apology("Password confirmation field is blank!")
        elif password != confirmation:
            return apology("Password and confirmation fields don't match!")
        elif check_password_hash(current_hash, password):
            return apology("New password can't be the same as the old password")

        # Set new password
        db.execute("UPDATE users SET hash = ? WHERE id = ?", generate_password_hash(password), session["user_id"])

        # Redirect to index
        flash("Password changed successfully")
        return redirect("/")

    else:
        return render_template("settings.html")
