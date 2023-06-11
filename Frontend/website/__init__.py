# this file, __init__ makes the folder a package
from flask import Flask

def create_app():
    app = Flask(__name__)
    # secret key for app security
    app.config["SECRET_KEY"] = "secret_key"

    from .views import views
    from .auth import auth

    app.register_blueprint(views, url_prefix="/")
    app.register_blueprint(auth, url_prefix="/")

    return app