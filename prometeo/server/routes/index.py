from server import app
from flask import render_template


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
    return render_template('/admin_bomberos.html')

@app.route('/admin_dispositivos')
def admin_dispositivos():
    return render_template('/admin_dispositivos.html')

@app.route('/admin_eventos')
def admin_eventos():
    return render_template('/admin_eventos.html')

@app.route('/misalud_estado')
def misalud_estado():
    return render_template('/misalud_estado.html')

@app.route('/misalud_informe')
def misalud_informe():
    return render_template('/misalud_informe.html')

@app.route('/admin_eventos_form')
def admin_eventos_form():
    return render_template('/admin_eventos_form.html')
