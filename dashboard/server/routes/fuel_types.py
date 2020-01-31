import requests
import json
import mysql.connector
from .configbdd import config
import logging

class fuelTypes(object):
    def __init__(self):
        self.logger = logging.getLogger('prometo.fuel_types.fuelTypes')
        self.logger.debug('creating an instance of fuelTypes')

    def get_allfueltypes(self):
        try:
            con = mysql.connector.connect(**config)
            cursor = con.cursor()

            self.logger.debug('calling the stored procedure')
            cursor.callproc('sp_select_all_fuel_types')
            for result in cursor.stored_results():
                data = result.fetchall()
            if len(data) > 0:
                self.logger.debug('there is data:')
                for i in data:
                    self.logger.debug(i)
                return(data)
            else:
                self.logger.debug('no data')
                return None
        except Exception as e:
            self.logger.error('error getting the data')
            return None

        finally:
            cursor.close()
            con.close()
