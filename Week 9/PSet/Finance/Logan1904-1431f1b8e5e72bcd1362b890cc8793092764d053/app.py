import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

from datetime import datetime

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

    # Check if transactions table exists
    exists = db.execute("SELECT name FROM sqlite_master WHERE type = 'table' AND name = 'transactions'")
    if not exists:
        return render_template("no_transactions.html")

    # Get user id
    user_id = session["user_id"]

    # Get all owned stocks
    old_stocks = db.execute("SELECT DISTINCT(symbol) FROM transactions WHERE user_id = ?", user_id)

    # Remove stocks with no shares
    stocks = [stock for stock in old_stocks if db.execute("SELECT SUM(shares) FROM transactions WHERE symbol=?",stock["symbol"])[0]["SUM(shares)"]]

    # Create count, price and value dictionaries for each stock as well as add to total
    count = {}
    price = {}
    value = {}
    for stock in stocks:
        count[stock["symbol"]] = db.execute("SELECT SUM(shares) FROM transactions WHERE user_id = ? AND symbol = ?", user_id, stock["symbol"])[0]["SUM(shares)"]
        price[stock["symbol"]] = lookup(stock["symbol"])["price"]
        value[stock["symbol"]] = count[stock["symbol"]] * price[stock["symbol"]]

    # Get user cash
    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

    # Return index page
    return render_template("index.html", stocks=stocks, count=count, price=price, value=value, cash=cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # GET
    if request.method == "GET":
        return render_template("buy.html")

    # Get user inputs
    symbol = request.form.get("symbol")
    shares = request.form.get("shares")

    print(shares)

    if not symbol:
        return apology("Symbol cannot be blank")
    elif not shares:
        return apology("Shares cannot be blank")

    try:
        shares = int(shares)
    except ValueError:
        return apology("Shares must be a positive integer")

    if shares < 1:
        return apology("Number of shares has to be positive")

    # Get stock values
    stock = lookup(symbol)
    if not stock:
        return apology("Symbol does not exist")

    # Get user cash
    user_id = session["user_id"]
    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

    # Validate
    if cash < shares*stock["price"]:
        return apology("You do not have enough cash")

    # Check if transactions table exists
    exists = db.execute("SELECT name FROM sqlite_master WHERE type = 'table' AND name = 'transactions'")
    if not exists:
        db.execute("CREATE TABLE transactions (id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, user_id INTEGER NOT NULL, symbol TEXT NOT NULL, price REAL NOT NULL, shares INTEGER NOT NULL, date TEXT NOT NULL, FOREIGN KEY(user_id) REFERENCES users(id))")


    # Add buy data to transactions
    db.execute("INSERT INTO transactions (user_id, symbol, price, shares, date) VALUES(?,?,?,?,?)", user_id, stock["symbol"], stock["price"], shares, datetime.today().strftime('%Y-%m-%d %H:%M:%S'))

    # Decrease user cash
    leftover = cash - shares*stock["price"]
    db.execute("UPDATE users SET cash = ? WHERE id = ?", leftover, user_id)

    return render_template("bought.html", stock=stock, shares=shares, cash=leftover)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Check if transactions table exists
    exists = db.execute("SELECT name FROM sqlite_master WHERE type = 'table' AND name = 'transactions'")
    if not exists:
        return render_template("no_transactions.html")

    # Get user transactions
    user_id = session["user_id"]
    transactions = db.execute("SELECT * FROM transactions WHERE user_id = ?", user_id);

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
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # GET
    if request.method == "GET":
        return render_template("quote.html")

    # Get stock symbol
    symbol = request.form.get("symbol")

    # Get stock values
    stock = lookup(symbol)

    # Validate
    if not symbol:
        return apology("Symbol cannot be blank")
    elif not stock:
        return apology("Symbol does not exist")

    # Return quoted stock
    return render_template("quoted.html", stock=stock)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # GET
    if request.method == "GET":
        return render_template("register.html")

    # Get username and password
    username = request.form.get("username")
    password = request.form.get("password")
    confirmation = request.form.get("confirmation")

    # Validate username and password
    if not username:
        return apology("Username cannot be blank")
    elif not password or not confirmation:
        return apology("Password cannot be blank")
    elif password != confirmation:
        return apology("Passwords have to match")

    # Form has
    hash = generate_password_hash(password)

    # Check if user already exists
    if db.execute("SELECT id FROM users WHERE username = ?", username):
        return apology("Username already exists")

    db.execute("INSERT INTO users (username,hash) VALUES(?,?)", username, hash)

    # Redirect to login
    return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # Check if transactions table exists
    exists = db.execute("SELECT name FROM sqlite_master WHERE type = 'table' AND name = 'transactions'")
    if not exists:
        return render_template("no_transactions.html")

    # Get user id
    user_id = session["user_id"]

    # Get user stocks
    stocks = db.execute("SELECT DISTINCT(symbol) FROM transactions WHERE user_id = ?", user_id)
    user_stocks = [x["symbol"] for x in stocks]

    # GET
    if request.method == "GET":
        return render_template("sell.html", stocks=user_stocks)

    # Get user inputs
    symbol = request.form.get("symbol").upper()
    shares_tosell = request.form.get("shares")

    if not symbol:
        return apology("Symbol cannot be blank")
    elif not shares_tosell:
        return apology("Shares cannot be blank")

    try:
        shares_tosell = int(shares_tosell)
    except ValueError:
        return apology("Shares must be a positive integer")

    if shares_tosell < 1:
        return apology("Number of shares has to be positive")

    # Get stock values
    stock = lookup(symbol)
    if not stock:
        return apology("Symbol does not exist")

    # Get user cash
    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]


    if symbol not in user_stocks:
        return apology("You do not own any of this stock")

    # Get user shares
    shares_user = db.execute("SELECT SUM(shares) FROM transactions WHERE user_id = ? AND symbol = ?", user_id, symbol)[0]["SUM(shares)"]
    if shares_tosell > shares_user:
        return apology("You do not have enough shares")

    # Add sell data to portfolio
    db.execute("INSERT INTO transactions (user_id, symbol, price, shares, date) VALUES(?,?,?,?,?)", user_id, stock["symbol"], stock["price"], -shares_tosell, datetime.today().strftime('%Y-%m-%d %H:%M:%S'))

    # Increase user cash
    leftover = cash + shares_tosell*stock["price"]
    db.execute("UPDATE users SET cash = ? WHERE id = ?", leftover, user_id)

    return render_template("sold.html", stock=stock,shares=shares_tosell, cash=leftover)
