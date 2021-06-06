from flask import render_template, jsonify
from DashboardWebApp import app
from DashboardWebApp.models import *
from flask_sqlalchemy import SQLAlchemy


@app.route('/get_alarms', methods=['GET'])
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
	
	return jsonify(records)


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
	

db.create_all()


