import json

class coreDecision(object):
    # the init method where we get the limit parameters from the parameters.json file
    def __init__(self):
        with open('parameters.json', 'r') as file:
            self.parametros = json.load(file)
            file.close()

    # Here are the rules based on the parameters
    # Input: temperature, humidity and CO ppm
    # Output: the status of the firefighter

    def get_status(self, temperature, humidity, co_ppm):
        if co_ppm <= int(self.parametros["limits_co"]["verde"]):
            status_co = 1
        elif co_ppm >= int(self.parametros["limits_co"]["rojo"]):
            status_co = 3
        else:
            status_co = 2

        if temperature <= int(self.parametros["limits_temperature"]["verde"]):
            status_temp = 1
        elif temperature >= int(self.parametros["limits_temperature"]["rojo"]):
            status_temp = 3
        else:
            status_temp = 2

        # 1 - green, 2 - yellow, 3 - red
        return max(status_co, status_temp)


# This code is to test the class
#prueba = coreDecision()
#print(prueba.parametros)
#print(prueba.parametros["limits_co"])
#print(prueba.parametros["limits_co"]["rojo"])
#print(prueba.get_status(41, 10, 20))
