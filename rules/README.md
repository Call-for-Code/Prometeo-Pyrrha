# Rules implementation of the firefighter status

This is a implementation of the firefighter status

The main file is core_decision_flask_app.py

To execute locally you can use the next command:

`FLASK_APP=core_decisioflask_app.py flask run`

Call example:
`curl -X POST "http://127.0.0.1:5000/coreDecision/core_decision" -H "accept: application/json" -H "Content-Type: application/json" -d "{ \"Temperature\": 10, \"Humidity\": 15, \"CO_ppm\": 20}"`

It returns a json with the Status of the firefighter:

* 1 - green
* 2 - yellow
* 3 - red
