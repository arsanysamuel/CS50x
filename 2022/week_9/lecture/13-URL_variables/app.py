from flask import Flask, redirect, url_for

app = Flask(__name__)

@app.route("/")
def index():
    return redirect(url_for("show_req"))  # url_for returns the route path of the function show_req

@app.route("/show/")
def show_req():
    return "Enter a number in the url"

@app.route("/show/<number>")  # Passing a variable through the url
def show(number):  # Passed from the url route above
    return f"You passed in {number}"