#!/usr/bin/python3
from DashboardWebApp import app

# adding host '0.0.0.0' & a port, this can serve as a local network server when running.
# type the following address in a browser to view the webapp:
# http://localhost:5003/
app.run(host="0.0.0.0", port=5003, debug=True)