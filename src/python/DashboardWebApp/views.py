from flask import render_template, jsonify, request
from DashboardWebApp import app
from DashboardWebApp.models import *
from flask_sqlalchemy import SQLAlchemy
import requests
import json

IOT_BUS_URL = "http://192.168.1.68/"

def get_behavior_records():
	"""
	Returns list of behavior records from db.
	"""
	return BehaviorRecord.query.all()


def get_behavior_record_values(date: str):
	"""
	Gets Behavior Record values for a specific date.
	"""
	try:
		rec = BehaviorRecord.query.filter_by(date=date).first()

		# Catch nonexistent record for specified date and create one.
		if rec is None:
			print(f"-i- Adding new record for {date}")
			rec = BehaviorRecord(happy_point_count=0, sad_point_count=0, bonus_point_count=0)
			db.session.add(rec)
			db.session.commit()

		#print(f"-i- {rec}")
		return rec

	except Exception as e:
		print(f'Error finding behavior records: {e}')

		return None


@app.route('/led_status', methods=['GET', 'POST'])
def get_led_status():
	"""
	Sends a request to the arduino to get the current state of the LED's.
	"""

	url = f"{IOT_BUS_URL}current_led_state"
	return json.loads(requests.get(url).text)


@app.route('/led_adjust/<state_irgb>', methods=['GET', 'POST'])
def adjust_led(state_irgb):
	"""
	Sends a request to the arduino to adjust the leds.
	"""
	state = state_irgb.split()[0]
	irgb = state_irgb.split()[1].upper()
	url = f"{IOT_BUS_URL}led?state={state};irgb={irgb};"
	return json.loads(requests.get(url).text)


@app.route('/trigger_alarm', methods=['GET', 'POST'])
def trigger_school_alarm():
	"""
	Sends a get request to the arduino to trigger the appropriate alarm.
	"""
	req = request.args.to_dict()
	msg = req["msg"]
	irgb = req["irgb"]
	name = req["melody"]
	url = f"{IOT_BUS_URL}alarm?name={name}&msg={msg}&irgb={irgb}"
	print(f"req: {req}")
	print(f"url {url}")
	return jsonify(requests.get(url).text)


@app.route('/datetime', methods=['GET'])
def get_current_datetime():
	"""
	Returns the current date & time to the arduino upon request.
	"""
	return get_datetime_string()


@app.route('/get_alarms', methods=['GET'])
def get_alarms_from_db():
	"""
	Queries the database for all alarms currently in the database
	"""
	db_records = Alarms.query.all()
	records = []
	for rec in db_records:
		temp_dict = rec.__dict__
		temp_dict.pop(list(temp_dict.keys())[0])
		records.append(temp_dict)
	
	return jsonify(records)


@app.route("/add_new_alarm")
def add_new_alarm(alarm: Alarms):
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
	

@app.route("/update_behavior_points/<update_opt>", methods=['GET', 'POST'])
def update_behavior_points(update_opt):
	"""
	Updates happy, sad, or bonus points for the current date behavior record.

	@param update_opt: update option
	@type update_opt: str
	"""
	rec: BehaviorRecord
	rec = get_behavior_record_values(get_date_string())
	if update_opt != "-1":
		if update_opt == "0":
			rec.happy_point_count += 1
		elif update_opt == "1" and rec.happy_point_count > 0:
			rec.happy_point_count -= 1
		elif update_opt == "2":
			rec.sad_point_count += 1
		elif update_opt == "3" and rec.sad_point_count > 0:
			rec.sad_point_count -= 1
		elif update_opt == "4":
			rec.bonus_point_count += 1
		elif update_opt == "5" and rec.bonus_point_count > 0:
			rec.bonus_point_count -= 1

		db.session.commit()

	return jsonify({"status": "success", "behavior_record": rec.payload})


@app.route("/")
def render_homepage():
	"""
	Starts homepage for the webapp.
	"""

	return render_template('homepage.html', alarms=Alarms.query.all())
	


