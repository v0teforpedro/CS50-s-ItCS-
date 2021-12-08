import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # assigning user_id to session
    user_id = session["user_id"]
    
    # SQL query to get data from "transactions" table, and to get user's cash amount from "users" table
    stocks = db.execute("SELECT symbol, name, SUM(shares) AS totalShares, price FROM transactions WHERE user_id = ? GROUP BY symbol HAVING totalShares > 0", user_id)
    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
    
    # making total equal cash (for now..)
    total = cash
    
    # in this for loop we lookup current price of stock (through lookup), and transfering this data back to "stocks". Also add to "total" price of stocks
    for stock in stocks:
        quote = lookup(stock["symbol"])
        stock["price"] = quote["price"]
        total += stock["price"] * stock["totalShares"]
    
    # we render html and transfering needed data to be used in jinja    
    return render_template("index.html", stocks=stocks, cash=usd(cash), total=usd(total), usd=usd)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    
    # assigning user_id to session
    user_id = session["user_id"]
    if request.method == "POST":
        
        # assigning symbol with user's input
        symbol = request.form.get("symbol")
        
        # assigning lookup function
        stock = lookup(symbol)
        
        # check if stock field is blank or not valid
        if not symbol or (stock == None):
            return apology("Invalid input")
        
        # checking that shares from user is int    
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("Invalid entry")
        
        # check that we have positive number of shares
        if shares < 1:
            return apology("Invalid number")
        
        # SQL query for current cash of the user
        cash_balance = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
        
        # assigning total price of stock user wants to buy
        total_price = shares * stock["price"]
        
        # check if user can afford this buy
        if cash_balance < total_price:
            return apology("Not enough funds")
        
        # updating cash balance and inserting transaction data
        else:
            db.execute("UPDATE users SET cash = ? WHERE id = ?", cash_balance - total_price, user_id)
            db.execute("INSERT INTO transactions (name, symbol, shares, price, action, user_id) VALUES(?, ?, ?, ?, ?, ?)", stock["name"], symbol, shares, stock["price"], 'bought', user_id)
        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    
    # assigning user_id to session
    user_id = session["user_id"]
    
    # getting all data from transaction table
    rows = db.execute("SELECT * FROM transactions WHERE user_id = ? ORDER BY time DESC", user_id)
    
    # rendering html
    return render_template("history.html", rows=rows, usd=usd)


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
    
    # via POST - render quoted.html
    if request.method == "POST":
        symbol = request.form.get("symbol")
        
        # check for valid input
        stock = lookup(symbol)
        if not symbol or (stock == None):
            return apology("Invalid input")
            
        # passing lookup function to html, and usd function to fomat price
        return render_template("quoted.html", stock=stock, format_price=usd)
    
    # via GET - render quote.html
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    
    if request.method == "POST":
        
        # checking username input
        username = request.form.get("username")
        
        if not username:
            return apology("Please provide username")
        
        # checking user's password input and match    
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        
        # PERSONAL TOUCH: "Require users’ passwords to have some number of letters, numbers, and/or symbols."
        if len(password) < 8 or len(password) > 16:
            return apology("Make password 8 to 16 characters long!")
        elif not any(char.isdigit() for char in password):
            return apology("Password should contain at least one digit number :(")
        elif not any(char.isupper() for char in password):
            return apology("Password should contain at least one upper case character :(")
        
        if not password or password != confirmation:
            return apology("Password doesn't match or absent")
        
        # hashing user's password
        hash = generate_password_hash(password)
        
        # following statement will try to exeute sql query, and if it isn't successful, it returns apology - which means username is already taken
        try:
            db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, hash)
            
            # Redirect user to home page
            return redirect("/")
        
        except:
            return apology("This username already exists, please try another username")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    
    # assigning user_id to session    
    user_id = session["user_id"]
    
    if request.method == "POST":
        
        # getting symbol from user
        symbol = request.form.get("symbol")

        # assigning lookup function
        quote = lookup(symbol)
        
        # check for valid input of symbol
        if not symbol or (quote == None):
            return apology("Invalid input")
        
        # check that shares is int
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("Invalid entry")
        
        # SQL query to show sum of shares user have for particular symbol    
        owned_shares = db.execute("SELECT SUM(shares) AS sumShares FROM transactions WHERE user_id = ? AND symbol = ?", user_id, symbol)[0]["sumShares"]
        
        # SQL query to get user's cash balance
        cash_balance = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
        
        # calculation of total price of sell transaction
        total_price = shares * quote["price"]
        
        # check if user has enough shares to sell
        if owned_shares < shares:
            return apology("You don't have that much shares")
        
        # check if input of shares is atleast 1
        elif shares < 1:
            return apology("Invalid input")
        
        # updating cash in "users" table, and adding new transaction data to "transactions" table
        else:
            db.execute("UPDATE users SET cash = ? WHERE id = ?", cash_balance + total_price, user_id)
            db.execute("INSERT INTO transactions (name, symbol, shares, price, action, user_id) VALUES(?, ?, ?, ?, ?, ?)", quote["name"], symbol, -shares, quote["price"], 'sold', user_id)
        return redirect("/")
    else:
        
        # make sure that only available (user actualy have it in balance) symbol is shown in sell.html
        rows = db.execute("SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0", user_id)
        return render_template("sell.html", rows=rows)
        


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
