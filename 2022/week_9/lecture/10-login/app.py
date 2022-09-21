from flask import Flask, redirect, render_template, request, session
from flask_session import Session

# Configure app
app = Flask(__name__)

# Configure session
app.config["SESSION_PERMANENT"] = False  # the cookies get deleted once logged out
app.config["SESSION_TYPE"] = "filesystem"  # use the filesystem on the server to store user cookies
Session(app)  # configure the app to use sessions


@app.route("/")
def index():
    if not session.get("name"):  # if no name value in the session redirect the user to the login route
        return redirect("/login")  # redirect to another route within the route backend
    return render_template("index.html")


@app.route("/login", methods=["GET", "POST"])
def login():
    if request.method == "POST":
        session["name"] = request.form.get("name")
        return redirect("/")
    return render_template("login.html")


@app.route("/logout")
def logout():
    session["name"] = None
    return redirect("/")
