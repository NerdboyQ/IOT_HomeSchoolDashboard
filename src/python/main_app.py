from models import *
from flask import Flask, render_template
from flask_sqlalchemy import SQLAlchemy


app = Flask(__name__)
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///alarms.sqlite3'
app.config['SECRET_KEY'] = "random string"
db = SQLAlchemy(app)
		

def get_alarms_from_db():
	"""
	Queries the database for all alarms currently in the database
	"""
	db_records = Alarm.query.all()
	records = []
	for rec in db_records:
		temp_dict = rec.__dict__
		temp_dict.pop(list(temp_dict.keys())[0])
		records.append(temp_dict)
	
	return records


@app.route("/add_new_alarm")
def add_new_alarm(alarm: Alarm):
	"""
	Stores an alarm model instance in the database.
	
	@param alarm: New Alarm instance
	@param alarm: alarm
	"""
	db.session.add(alarm)
	db.session.commit()


@app.route("/remove_alarm")
def remove_alarm(alarms: list):
	"""
	Removes one or more alarms from the database.
	
	@param alarms: list of one or more alarms to remove by name
	@type alarms: list
	"""
	if alarms[0] == '_ALL_':
		Alarm.query.delete()
	else:
		for alarm_name in alarms:
			Alarm.query.filter_by(name=alarm_name).delete()
	

@app.route("/")
def render_homepage():
	"""
	Starts homepage for the webapp.
	"""
	return render_template('homepage.html', alarms=Alarm.query.all())
	

if __name__ == "__main__":
	# adding host '0.0.0.0' & a port, this can serve as a local network server when running.
	db.create_all()
	app.run(host="0.0.0.0", port=5003, debug=True)
