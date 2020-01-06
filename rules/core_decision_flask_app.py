from flask import Flask, Response, jsonify
from flask_restplus import Api, Resource, fields, reqparse
from flask_cors import CORS, cross_origin
import os
from coreDecision import coreDecision

# The application
app = Flask(__name__)
CORS(app)

api_core_decision = Api(app, version='1.0', title='API to ask for the status of the firefighter', validate=False)
ns = api_core_decision.namespace('coreDecision', 'API to ask for the status of the firefighter')

# we define the input data as Float
model_input = api_core_decision.model('Enter the data:', {"Temperature": fields.Float, "Humidity": fields.Float, "CO_ppm": fields.Float})

# On Bluemix, get the port number from the environment variable PORT
# When running this app on the local machine, default to 8080
port = int(os.getenv('PORT', 8080))

# We initialize the status variable. Ii will load the parameters
statusFirefighter = coreDecision()


# The ENDPOINT
@ns.route('/core_decision')

class core_decision(Resource):
    @api_core_decision.response(200, "Success", model_input)
    @api_core_decision.expect(model_input)
    def post(self):
        # We prepare the arguments
        parser = reqparse.RequestParser()
        parser.add_argument('Temperature', type=float)
        parser.add_argument('Humidity', type=float)
        parser.add_argument('CO_ppm', type=float)
        args = parser.parse_args()

#       In case we want to test the call
#        result = statusFirefighter.get_status(10, 20, 30)

        # We call to the get_status method in order to get the status of the firefighter
        status = statusFirefighter.get_status(float(args["Temperature"]), float(args["Humidity"]), float(args["CO_ppm"]))

        # We return the status in a json format
        return jsonify({"Status": status})
#run if __name__ == '__main__': app.run(host='0.0.0.0', port=port, debug=False) # deploy with debug=False
