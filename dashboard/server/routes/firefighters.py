#import requests
import json
import mysql.connector
from .configbdd import config

class firefighters(object):

    def insert_firefighter(self, bomberoid, nombre, apellidos, email):
        try:
            con = mysql.connector.connect(**config)
            cursor = con.cursor()

            cursor.callproc('sp_create_firefighter', (bomberoid, nombre, apellidos, email))

            for result in cursor.stored_results():
                data = result.fetchall()

            if len(data[0][0]) is 0:
                con.commit()
                return True
            else:
                return False

        except Exception as e:
            return None

        finally:
            cursor.close()
            con.close()


    def update_firefighter(self, bomberoid, nombre, apellidos, email):
        try:
            con = mysql.connector.connect(**config)
            cursor = con.cursor()

            cursor.callproc('sp_update_firefighter', (bomberoid, nombre, apellidos, email))

            for result in cursor.stored_results():
                data = result.fetchall()

            if len(data[0][0]) is 0:
                con.commit()
                return True
            else:
                return False

        except Exception as e:
            return None

        finally:
            cursor.close()
            con.close()

    def get_firefighter(self, bomberoid):
        try:
            con = mysql.connector.connect(**config)
            cursor = con.cursor()

            cursor.callproc('sp_select_firefighter', (bomberoid))

            for result in cursor.stored_results():
                data = result.fetchall()

            if len(data) > 0:
                return(data[0])
            else:
                return None

        except Exception as e:
            return None

        finally:
            cursor.close()
            con.close()

    def get_allfirefighters(self):
        print("get_alldevices - entro en la funcion")

        try:
            con = mysql.connector.connect(**config)
            cursor = con.cursor()

            print("get_alldevices - llamada a sql")
            cursor.callproc('sp_select_all_firefighters')
            for result in cursor.stored_results():
                data = result.fetchall()
            if len(data) > 0:
                print("get_alldevices - Hay informacion")
                for i in data:
                    print(i)
                return(data)
            else:
                print("get_alldevices - NO HAY INFORMACION")
                return None
        except Exception as e:
            print("get_alldevices - Estoy en la excepcion")
            return None

        finally:
            cursor.close()
            con.close()
