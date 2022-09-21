'''
To run this server execute:
    $ flask run
'''
from flask import Flask, render_template, request

app = Flask(__name__)  # passing the name of the current file, telling the framework to turn this file to a flask applicaition

@app.route("/")  # decorator, telling flask to call this function when the route is the root route
def index():
    return render_template("index.html")  # render this file to the screen, passing the username variable into name vaiable in html

@app.route("/greet", methods=["POST"])  # adding post method support to this route
def greet():
    name = request.form.get("username", "world")  # request.args: GET requests, request.form: POST requests
    return render_template("greet.html", name=name)