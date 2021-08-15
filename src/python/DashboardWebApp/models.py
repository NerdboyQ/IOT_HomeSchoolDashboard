from DashboardWebApp import db
from datetime import datetime as dt


def get_date_string():
	"""
	Gets the current date and converts into a more user friendly string format.
	"""
	return dt.now().date().strftime("%b %d, %Y")


def get_datetime_string():
	"""
	Gets the current date & time then converts into a more user friendly string format.
	"""
	return dt.now().time().strftime("%I:%M%p") + dt.now().date().strftime(" %b %d, %Y")


class BehaviorRecord(db.Model):
	"""
	BehaviorRecord model for recording happy, sad, or bonus points.
	"""
	date = db.Column(db.String(120), default=get_date_string, primary_key=True)
	happy_point_count = db.Column(db.Integer, nullable=False)
	sad_point_count = db.Column(db.Integer, nullable=False)
	bonus_point_count = db.Column(db.Integer, nullable=False)

	def __repr__(self):
		""""
		Returns field date of behavior record.
		"""
		return f'<BehaviorRecord: {self.date}>'

	@property
	def payload(self):
		"""
		Returns the dictionary representation of the Baavior Record instance.
		"""
		_payload = {
			"date" : self.date,
			"happy_point_count": self.happy_point_count,
			"sad_point_count": self.sad_point_count,
			"bonus_point_count": self.bonus_point_count
		}
		return _payload


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
		@rtype dict
		"""

		return self.__dict__
