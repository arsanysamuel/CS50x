from flask import Flask, render_template, request

SPORTS = [
    "Basketball",
    "Soccer",
    "Ultimate Frisbee"
    ]

app = Flask(__name__)

@app.route('/')
def index():
    return render_template("index.html", sports=SPORTS)  # passing SPORTS to the variable sports

@app.route("/register", methods=["POST"])
def register():
    # Validate submission
    name = request.form.get("name")
    sport = request.form.get("sport")
    if not name or not sport in SPORTS:
        return render_template("failure.html")
    return render_template("success.html")
