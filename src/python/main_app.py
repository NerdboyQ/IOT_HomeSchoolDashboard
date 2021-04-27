from flask import Flask, render_template
from flask_sqlalchemy import SQLAlchemy
from datetime import datetime as dt

app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///alarms.sqlite3'
app.config['SECRET_KEY'] = "random string"
db = SQLAlchemy(app)
	
def get_date_string():
	"""
	Gets the current date and converts into a more user friendly string format.
	"""
	return dt.now().date().strftime("%b %d, %Y")
		
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

class Alarm(db.Model):
	"""
	Alarm model for storing alarm settings int eh webapp database.
	"""
	name = db.Column(db.String(120), default="Alarm", nullable=False, primary_key=True)
	time = db.Column(db.String(120), default="8:30am")
	start_date = db.Column(db.String(120), default=get_date_string)
	alarm_melody = db.Column(db.String(120), default="Drankin Patna")
	repeat = db.Column(db.String(120), default="YES")
	days = db.Column(db.String(27), default="ALL")
	color = db.Column(db.String(7), default="#ff0000")
	
	def __repr__(self):
		"""
		Returns the name of the Alarm instance
		"""
		return f"<Alarm: {self.name}>"
		
	@property
	def payload(self):
		"""
		Returns the dictionary representation of the Alarm model instance.
		
		@return alarm object
		@rtpe dict
		"""
		return self.__dict__

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
	
	@param alarm: list of one or more alarms to remove by name
	@type alarm: list
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
	print(get_alarms_from_db())
	app.run(host="0.0.0.0", port=5003, debug=True)
