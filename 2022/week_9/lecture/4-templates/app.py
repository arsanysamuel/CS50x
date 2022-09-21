'''
To run this server execute:
    $ flask run
'''
from flask import Flask, render_template, request

app = Flask(__name__)  # passing the name of the current file, telling the framework to turn this file to a flask applicaition

@app.route("/")  # decorator, telling flask to call this function when the route is the root route
def index():
    return render_template("index.html")  # render this file to the screen, passing the username variable into name vaiable in html

@app.route("/greet")
def greet():
    name = request.args.get("username", "world")  # if no value for the username parameter default to the 2nd arg
    return render_template("greet.html", name=name)