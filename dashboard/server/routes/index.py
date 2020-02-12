from server import app
import logging
from flask import render_template, request, flash, Flask, session, redirect, url_for
import mysql.connector
from flask import jsonify
from .devices import devices
from .firefighters import firefighters
from .events import event
from .configbdd import config
from .fuel_types import fuelTypes
from .status import status
from .event_types import eventTypes
from .metrics import metrics

# create logger with 'prometeo'
logger = logging.getLogger('prometeo')
logger.setLevel(logging.DEBUG)

app.secret_key = 'development key'

all_fueltypes = fuelTypes().get_allfueltypes()
all_status = status().get_allstatus()
all_event_types = eventTypes().get_alleventtypes()


@app.errorhandler(404)
@app.route("/error404")
def page_not_found(error):
    return app.send_static_file('404.html')

@app.errorhandler(500)
@app.route("/error500")
def requests_error(error):
    return app.send_static_file('500.html')

@app.route('/index')
@app.route('/')
def index():
    return render_template('index.html')

@app.route('/admin_bomberos')
def admin_bomberos():
    bombero = firefighters()
    return render_template('/admin_bomberos.html', bomberos=bombero.get_allfirefighters())

@app.route('/admin_dispositivos')
def admin_dispositivos():
    dispositivo = devices()
    return render_template('/admin_dispositivos.html', dispositivos=dispositivo.get_alldevices())

@app.route('/add_dispositivo', methods=['POST'])
def add_dispositivo(sensorid, model, version):
    logger.info('Adding a new devevice')
    dispositivo = devices()
    dispositivo.insert_device(sensorid, model, version)

@app.route('/admin_eventos')
def admin_eventos():
    evento = event()
    return render_template('/admin_eventos.html', eventos = evento.get_allevents())

@app.route('/admin_eventos_form', methods=['GET'])
def admin_eventos_form():
    eventid = request.args.get('eventid')
    obj_evento = event()
    datos_evento = obj_evento.get_event(eventid)
    event_firefighters_devices = obj_evento.get_event_firefighters_devices(eventid)

    return render_template('admin_eventos_form.html', datos_evento=datos_evento, firefighters_devices=event_firefighters_devices, all_status=all_status, all_fueltypes=all_fueltypes, all_event_types=all_event_types)

@app.route('/misalud_estado')
def misalud_estado():
    return render_template('/misalud_estado.html')

@app.route('/misalud_informe')
def misalud_informe():
    return render_template('/misalud_informe.html')

@app.route('/report_mediciones')
def report_mediciones():
    max_mediciones = 5
    evento = event()
    eventid = request.args.get('eventid')
#    eventid = 1
    obj_evento = event()
    datos_evento = obj_evento.get_event(eventid)
    bomberos_dispositivos = obj_evento.get_event_firefighters_devices(eventid)
    metricas = metrics()
    metricas_bomberos = []
    longitudes = []
    for record_bd in bomberos_dispositivos:
        metricas_dispositivo = metricas.get_allmetrics(record_bd[3], datos_evento[5], max_mediciones)
        longitudes.append(len(metricas_dispositivo))
        metricas_bomberos.append(metricas_dispositivo)
    return render_template('/report_mediciones.html', bomberos_dispositivos=bomberos_dispositivos, longitudes=longitudes, metricas_bomberos=metricas_bomberos)


@app.route('/report_mediciones_new')
def report_mediciones_new():
    max_mediciones = 20
    evento = event()
#    eventid = request.args.get('eventid')
    eventid = 2
    obj_evento = event()
    datos_evento = obj_evento.get_event(eventid)
    bomberos_dispositivos = obj_evento.get_event_firefighters_devices(eventid)
    metricas = metrics()

    d=[]
    l = []
    d1 = metricas.get_allmetrics("0001", datos_evento[5], max_mediciones)
    d2 = metricas.get_allmetrics("0002", datos_evento[5], max_mediciones)
    d3 = metricas.get_allmetrics("0003", datos_evento[5], max_mediciones)
#    d4 = metricas.get_allmetrics("0004", datos_evento[5], max_mediciones)
#    d5 = metricas.get_allmetrics("0005", datos_evento[5], max_mediciones)
    d.append(d1)
    d.append(d2)
    d.append(d3)
#    d.append(d4)
#    d.append(d5)
#    d6 = metricas.get_allmetrics("0006", datos_evento[5], max_mediciones)
#    d7 = metricas.get_allmetrics("0007", datos_evento[5], max_mediciones)
#    d8 = metricas.get_allmetrics("0008", datos_evento[5], max_mediciones)
#    d9 = metricas.get_allmetrics("0009", datos_evento[5], max_mediciones)
#    d10 = metricas.get_allmetrics("0010", datos_evento[5], max_mediciones)
    l1 = len(d1)
    l2 = len(d2)
    l3 = len(d3)
#    l4 = len(d4)
#    l5 = len(d5)
    l.append(l1)
    l.append(l2)
    l.append(l3)
#    l.append(l4)
#    l.append(l5)
#    l6 = len(d6)
#    l7 = len(d7)
#    l8 = len(d8)
#    l9 = len(d9)
#    l10 = len(d10)

    return render_template('/report_mediciones_new.html', d=d, l=l)


@app.route('/mapa_evento')
def mapa_evento():
    latitud = request.args.get('latitud')
    longitud = request.args.get('longitud')
    return render_template('/mapa_evento.html', longitud=longitud, latitud=latitud)

@app.route('/testdb')
def testdb():

    con = mysql.connector.connect(**config)
    cursor = con.cursor()
    cursor.execute("SHOW DATABASES")

    for row in cursor:
        print(row[0])

    cursor.close()
    con.close()

    state = {"status": "Connection to Database OK"}
    return jsonify(state)
