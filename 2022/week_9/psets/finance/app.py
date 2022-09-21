import os
from datetime import datetime
from decimal import Decimal

from flask import Flask, flash, redirect, render_template, request, session, jsonify
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from sqlalchemy.ext.automap import automap_base
from sqlalchemy.orm import sessionmaker
from sqlalchemy.exc import IntegrityError
from sqlalchemy import create_engine, select

from helpers import apology, login_required, lookup, usd, is_password_invalid

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

# Configure SQLAlchemy library to use SQLite database
Base = automap_base()  # map already made database
engine = create_engine("sqlite:///finance.db")
Base.prepare(autoload_with=engine)

# Configuring SQLAlchemy Session object
SQLSession = sessionmaker(engine)
sql_session = SQLSession()

# Get SQL objects to be use
User = Base.classes.users
Asset = Base.classes.assets
Stock = Base.classes.stocks
Transaction = Base.classes.transactions

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
    with sql_session as s:  # opening sql session
        # Get the current user
        statement = select(User).filter_by(id=session["user_id"])
        current_user = s.execute(statement).scalars().one()

        # Getting assets info
        statement = select(Asset, Stock).join(Stock, Asset.stock_id == Stock.id,
                                              isouter=True).filter(Asset.user_id == current_user.id)
        results = s.execute(statement).all()  # will return a list of tuples, each tuple has 2 row objects for each joined table

        # Getting current stock prices and assets
        assets = []
        total = float(current_user.cash)
        for (asset, stock) in results:
            quote = lookup(stock.symbol)
            asset.price = quote["price"]
            total += quote["price"] * asset.shares
            assets.append(asset.__dict__ | stock.__dict__)  # Converting row objects to dictionary and merging them using |

    return render_template("index.html", assets=assets, cash=current_user.cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # Getting user input and other info
        symbol = request.form.get("symbol").upper()
        shares = request.form.get("shares")
        quote = lookup(symbol)

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

        with sql_session as s:
            # Get the current user
            statement = select(User).filter_by(id=session["user_id"])
            current_user = s.execute(statement).scalars().one()

            # Ensure user can afford purchase
            purchase_value = shares * quote["price"]
            if current_user.cash < purchase_value:
                return apology("Can't afford")

            # Add stock info to stocks table if doesn't exist
            try:
                stock = Stock(symbol=symbol, name=quote["name"])
                s.add(stock)
                s.commit()
            except IntegrityError:
                s.rollback()  # rollback session after an error
                statement = select(Stock).filter_by(symbol=symbol)
                stock = s.execute(statement).scalars().one()

            # Insert the purchase info to the transactions table
            transaction = Transaction(
                user_id=current_user.id,
                stock_id=stock.id,
                price=quote["price"],
                shares=shares,
                time=datetime.utcnow()
            )
            s.add(transaction)
            s.commit()

            # Subtract purchase value from user cash
            current_user.cash -= Decimal(purchase_value)
            s.commit()

            # Add shares to user assets, created unique index on user_id and stock_id to avoid duplicates
            try:
                asset = Asset(user_id=current_user.id, stock_id=stock.id, shares=shares)
                s.add(asset)
                s.commit()
            except IntegrityError:
                s.rollback()
                statement = select(Asset).filter(Asset.user_id == current_user.id and Asset.stock_id == stock.id)
                asset = s.execute(statement).scalars().one()
                asset.shares += shares
                s.commit()

        flash("Purchase completed")
        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    transactions = []
    with sql_session as s:
        # Get the current user
        statement = select(User).filter_by(id=session["user_id"])
        current_user = s.execute(statement).scalars().one()

        # Get transactions data
        statement = select(Transaction, Stock).join(Stock, Transaction.stock_id == Stock.id, isouter=True
                                                    ).filter(Transaction.user_id == current_user.id)
        results = s.execute(statement).all()
        for (stock, transaction) in results:
            transactions.append(stock.__dict__ | transaction.__dict__)

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
        with sql_session as s:
            statement = select(User).filter_by(username=request.form.get("username"))
            try:
                current_user = s.execute(statement).scalars().one()
            except:
                return apology("Invalid username", 403)

        # Ensure username exists and password is correct
        if not check_password_hash(current_user.hash, request.form.get("password")):
            return apology("invalid password", 403)

        # Remember which user has logged in
        session["user_id"] = current_user.id

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
        with sql_session as s:
            statement = select(User.username).filter_by(username=username)
            results = s.execute(statement).scalars().all()
            if len(results) > 0:
                return apology("Username already exists!")

        # Ensure password was submitted
        if password == "":
            return apology("Password field is blank!")
        elif confirmation == "":
            return apology("Password confirmation field is blank!")
        elif password != confirmation:
            return apology("Password and confirmation fields don't match!")

        # Check password characters
        invalid_apology = is_password_invalid(password)
        if invalid_apology is not None:
            return invalid_apology

        # If all goes well, store user credentials and login
        with sql_session as s:
            current_user = User(username=username, hash=generate_password_hash(password))
            s.add(current_user)
            s.commit()

            # Login The new user
            session.clear()
            session["user_id"] = current_user.id

        # Redirect to index
        flash("You've successfully registered.")
        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        shares = int(request.form.get("shares"))

        with sql_session as s:
            # Get the current user
            statement = select(User).filter_by(id=session["user_id"])
            current_user = s.execute(statement).scalars().one()

            # Get all symbols from user's assets
            statement = select(Asset.stock_id).filter_by(user_id=current_user.id)
            stock_ids = s.execute(statement).scalars().all()
            statement = select(Stock.symbol).filter(Stock.id.in_(stock_ids))  # Using in_() for IN clause
            symbols = s.execute(statement).scalars().all()

            # Check for symbol input
            if not symbol or symbol == "":
                return apology("Missing symbol")
            elif symbol not in symbols:
                return apology("Invalid symbol")

            # Check for shares input
            statement = select(Stock).filter(Stock.symbol == symbol)
            stock = s.execute(statement).scalars().one()
            statement = select(Asset).filter(Asset.user_id == current_user.id and Asset.stock_id == stock.id)
            asset = s.execute(statement).scalars().one()

            # Check use input share number
            if shares <= 0:
                return apology("Invalid number of shares")
            elif shares > asset.shares:
                return apology("Not enough shares in your assets")

            # Get current share price
            price = lookup(symbol)["price"]

            # Subtract shares from assets, delete asset if no more shares
            if asset.shares - shares == 0:
                s.delete(asset)
            else:
                asset.shares -= shares

            # Add cash to user
            current_user.cash += Decimal(price * shares)
            s.commit()

            # Add transaction
            transaction = Transaction(
                user_id=current_user.id,
                stock_id=stock.id,
                price=price,
                shares=shares,
                time=datetime.utcnow()
            )
            s.add(transaction)
            s.commit()

        # Return to index
        flash("Sold shares")
        return redirect("/")

    else:
        with sql_session as s:
            # Get the current user
            statement = select(User).filter_by(id=session["user_id"])
            current_user = s.execute(statement).scalars().one()

            # Get stocks symbols
            statement = select(Asset.stock_id).filter_by(user_id=current_user.id)
            stock_ids = s.execute(statement).scalars().all()
            statement = select(Stock.symbol).filter(Stock.id.in_(stock_ids))  # Using in_() for IN clause
            symbols = s.execute(statement).scalars().all()

        return render_template("sell.html", symbols=symbols)


@app.route("/get-shares")
def get_shares():
    '''Get max share number to set the numeric input max'''

    # Get the symbol from the get request
    symbol = request.args.get("symbol")

    with sql_session as s:
        # Get the current user
        statement = select(User).filter_by(id=session["user_id"])
        current_user = s.execute(statement).scalars().one()

        # Get shares number from the database
        statement = select(Stock.id).filter_by(symbol=symbol)
        stock_id = s.execute(statement).scalars().one()
        statement = select(Asset.shares).filter_by(stock_id=stock_id, user_id=current_user.id)
        shares = s.execute(statement).scalars().one()

    # Return the shares value
    return jsonify({"max": str(shares)})


@app.route("/settings", methods=["GET", "POST"])
def settings():
    """Change account password (just this for now)"""

    if request.method == "POST":

        # Getting form inputs
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        with sql_session as s:
            # Get the current user
            statement = select(User).filter_by(id=session["user_id"])
            current_user = s.execute(statement).scalars().one()
            # Check the old password
            if not check_password_hash(current_user.hash, request.form.get("old_password")):
                return apology("Password incorrect")

            # Check new password
            if password == "":
                return apology("Password field is blank!")
            elif confirmation == "":
                return apology("Password confirmation field is blank!")
            elif password != confirmation:
                return apology("Password and confirmation fields don't match!")
            elif check_password_hash(current_user.hash, password):
                return apology("New password can't be the same as the old password")

            # Check new password characters
            invalid_apology = is_password_invalid(password)
            if invalid_apology is not None:
                return invalid_apology

            # Set new password
            current_user.hash = generate_password_hash(password)
            s.commit()

        # Redirect to index
        flash("Password changed successfully")
        return redirect("/")

    else:
        return render_template("settings.html")
