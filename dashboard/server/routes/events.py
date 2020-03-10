#import requests
import json
import mysql.connector
from .configbdd import config

class event(object):

    def insert_event(self, data):
        try:
            con = mysql.connector.connect(**config)
            cursor = con.cursor()

            cursor.callproc('sp_create_event', (data))

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


    def update_event(self, data):
        try:
            con = mysql.connector.connect(**config)
            cursor = con.cursor()

            cursor.callproc('sp_update_event', (data))

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

    def get_event(self, eventid):
        try:
            con = mysql.connector.connect(**config)
            cursor = con.cursor()
            print("get_event")
            print(eventid)

            cursor.callproc('sp_select_event', (eventid,))

            print("get_event - he abierto el cursor")

            for result in cursor.stored_results():
                data = result.fetchall()


            if len(data) > 0:
                return(data[0])
            else:
                return None

        except Exception as e:
            print("get_event - estoy en la excepcion")
            return None

        finally:
            cursor.close()
            con.close()

    def get_allevents(self):
        print("get_allevents - entro en la funcion")

        try:
            con = mysql.connector.connect(**config)
            cursor = con.cursor()

            print("get_allevents - llamada a sql")
            cursor.callproc('sp_select_all_events')
            for result in cursor.stored_results():
                data = result.fetchall()
            if len(data) > 0:
                print("get_allevents - Hay informacion")
                for i in data:
                    print(i)
                return(data)
            else:
                print("get_allevents - NO HAY INFORMACION")
                return None
        except Exception as e:
            print("get_allevents - Estoy en la excepcion")
            return None

        finally:
            cursor.close()
            con.close()

    def get_event_firefighters_devices(self, eventid):
        try:
            con = mysql.connector.connect(**config)
            cursor = con.cursor()
            print("get_event")
            print(eventid)

            cursor.callproc('sp_select_event_firefighters_devices', (eventid,))

            print("get_event_firefighters_devices - he abierto el cursor")

            for result in cursor.stored_results():
                data = result.fetchall()


            if len(data) > 0:
                print("get_event_firefighters_devices - hay datos")
                for i in data:
                    print(i)
                return(data)
            else:
                print("get_event_firefighters_devices - no hay datos")
                return None

        except Exception as e:
            print("get_event_firefighters_devices - estoy en la excepcion")
            return None

        finally:
            cursor.close()
            con.close()
