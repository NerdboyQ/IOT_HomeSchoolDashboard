from flask import Flask

app = Flask(__name__)

@app.route("/")
def render_homepage():
	"""
	Starts homepage for the webapp.
	"""
	return "This is the homepage!"
	
if __name__ == "__main__":
	# adding host '0.0.0.0' & a port, this can serve as a local network server when running.
    app.run(host="0.0.0.0", port=5003, debug=True)