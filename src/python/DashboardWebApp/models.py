from DashboardWebApp import db
from datetime import datetime as dt


def get_date_string():
	"""
	Gets the current date and converts into a more user friendly string format.
	"""
	return dt.now().date().strftime("%b %d, %Y")


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
