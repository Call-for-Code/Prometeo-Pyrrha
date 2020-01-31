import requests
import json
import mysql.connector
from .configbdd import config

class status(object):

    def get_allstatus(self):
        print("get_allstatus - entro en la función")

        try:
            con = mysql.connector.connect(**config)
            cursor = con.cursor()

            print("get_allstatus - llamada a sql")
            cursor.callproc('sp_select_all_status')
            for result in cursor.stored_results():
                data = result.fetchall()
            if len(data) > 0:
                print("get_allstatus - Hay informacion")
                for i in data:
                    print(i)
                return(data)
            else:
                print("get_allstatus - NO HAY INFORMACION")
                return None
        except Exception as e:
            print("get_allstatus - Estoy en la excepción")
            return None

        finally:
            cursor.close()
            con.close()
