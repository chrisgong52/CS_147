from flask import Blueprint, render_template, request, flash
import requests

auth = Blueprint("auth", __name__)

@auth.route("login", methods=["GET", "POST"])
def login():
    data = request.form
    print(data)
    return render_template("login.html")
    # return "<p>Login</p>"

@auth.route("logout", methods=["GET", "POST"])
def logout():
    # return render_template("logout.html")
    return "<p>Logout</p>"

@auth.route("sign-up", methods=["GET", "POST"])
def sign_up():
    if request.method == "POST":
        email = request.form.get("email")
        firstName = request.form.get("firstName")
        password1 = request.form.get("password1")
        password2 = request.form.get("password2")
        if len(email) < 4:
            flash("Email must be greater than 4 characters.", category = "error")
        elif len(firstName) < 2:
            flash("First name must be greater than 2 characters.", category = "error")
        elif len(password1) < 3:
            flash("Password name must be greater than 3 characters.", category = "error")
        elif password1 != password2:
            flash("Passwords must match", category = "error")
        else:
            flash("Account created!", category = "success")
            # add user to db
        payload = {"var": 10}
        r = requests.get("http://3.21.98.211:5000/", params=payload)
        # p = requests.post("http://3.21.98.211:5000/", params=payload)
    return render_template("sign_up.html")
    # return "<p>Sign Up</p>"