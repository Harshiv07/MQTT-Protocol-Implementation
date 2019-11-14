import paho.mqtt.client as mqtt
from flask import Flask, render_template, request, redirect, url_for
from flask_socketio import SocketIO, emit
import csv

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app)

with open('data1.csv', mode='w') as data1:
    data1_writer = csv.writer(data1, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)


# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("/esp8266/temperature1")
    client.subscribe("/esp8266/humidity1")
    client.subscribe("/esp8266/ldr1")

# The callback for when a PUBLISH message is received from the ESP8266.
def on_message(client, userdata, message):
    #socketio.emit('my variable')
    print("Received message '" + str(message.payload) + "' on topic '"
        + message.topic + "' with QoS " + str(message.qos))
    if message.topic == "/esp8266/temperature1":
        data1_writer.writerow(['temperature1', 'dht_temperature1'])
        print("temperature update")
	socketio.emit('dht_temperature1', {'data': message.payload})
    if message.topic == "/esp8266/humidity1":
        data_writer.writerow(['humidity1', 'dht_humidity1'])
        print("humidity update")
	socketio.emit('dht_humidity1', {'data': message.payload})
    if message.topic == "/esp8266/ldr1":
        data_writer.writerow(['ldr', 'ldr_value1'])
        print("ldr update")
	socketio.emit('ldr_value1', {'data': message.payload})

mqttc=mqtt.Client()
mqttc.on_connect = on_connect
mqttc.on_message = on_message
mqttc.connect("localhost",1883,60)
mqttc.loop_start()

# Create a dictionary called pins to store the pin number, name, and pin state:
pins = {
   4 : {'name' : 'GPIO 4', 'board' : 'esp8266', 'topic' : 'esp8266/4', 'state' : 'False'},
   5 : {'name' : 'GPIO 5', 'board' : 'esp8266', 'topic' : 'esp8266/5', 'state' : 'False'}
   }

# Put the pin dictionary into the template data dictionary:
templateData = {
   'pins' : pins
   }

@app.route("/")
def main():
    return render_template('main.html', async_mode=socketio.async_mode, **templateData)

@app.route("/foo", methods=["GET", "POST"])
def foo():
   # Pass the template data into the template main.html and return it to the user
   return render_template('main.html', async_mode=socketio.async_mode, **templateData)

@app.route("/submit", methods=["POST"])
def get_form():
    if request.method=="POST":
        val = (request.form.get("sldval"))
        sx = "submit"
        print(val)
        mqttc.publish(sx ,val)
    return render_template('main.html', **templateData)

# The function below is executed when someone requests a URL with the pin number and action in it:
@app.route("/<board>/<changePin>/<action>")
def action(board, changePin, action):
   # Convert the pin from the URL into an integer:
   changePin = int(changePin)
   # Get the device name for the pin being changed:
   devicePin = pins[changePin]['name']
   # If the action part of the URL is "1" execute the code indented below:
   if action == "1" and board == 'esp8266':
      mqttc.publish(pins[changePin]['topic'],"1client1")
      pins[changePin]['state'] = 'True'
      print(pins[changePin]['topic'])
   if action == "0" and board == 'esp8266':
      mqttc.publish(pins[changePin]['topic'],"0client1")
      pins[changePin]['state'] = 'False'
      print(pins[changePin]['topic'])
   # Along with the pin dictionary, put the message into the template data dictionary:
   #templateData = {
   #   'pins' : pinshttp://0.0.0.0:6024/
   #}
   return render_template('main.html', **templateData)

@socketio.on('my event')
def handle_my_custom_event(json):
    print('received json data here: ' + str(json))

if __name__ == "__main__":
   socketio.run(app, host='0.0.0.0', port=6002, debug=True)
