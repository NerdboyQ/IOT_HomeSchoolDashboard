from flask import Flask
from flask_login import LoginManager
from flask_sqlalchemy import SQLAlchemy

app = Flask(__name__)
# Uses __init__.py file for app configuration
app.config.from_object('config')
# Database setup
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///alarms.sqlite3' # Creates table
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False # ignores any warning messages
db = SQLAlchemy(app)

from DashboardWebApp import views, models