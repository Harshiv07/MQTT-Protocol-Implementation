# MQTT-Protocol-implementation
##Implementing MQTT protocol using Raspberry pi 3(Server) and esp8266(Client)
Before you continue reading this project, please make sure you have Raspbian Operating System installed in your Raspberry Pi.
The Raspberry Pi is going to interact with the ESP8266 with the MQTT protocol. Having Mosquitto broker installed, you need to have Mosquitto broker running on the background:
```bash
pi@raspberry:~ $ mosquitto -d
```

##Installing Flask
We’re going to use a Python microframework called Flask to turn the Raspberry Pi into web server.
To install Flask, you’ll need to have pip installed. Run the following commands to update your Pi and install pip:

```bash
pi@raspberrypi ~ $ sudo apt-get update
pi@raspberrypi ~ $ sudo apt-get upgrade
pi@raspberrypi ~ $ sudo apt-get install python-pip python-flask git-core
```
###Then, you use pip to install Flask and Paho MQTT:

pi@raspberrypi ~ $ sudo pip install flask
pi@raspberrypi ~ $ sudo pip install paho-mqtt

##Installing SocketIO

This project uses SocketIO which allows you to create a Python Flask web page that can be asynchronously updated by your Python Flask application. This means that you don’t need to refresh the web page to see the latest readings, they are instantly updated. You’ll be installing the Flask SocketIO Python package.

```bash
pi@raspberrypi ~ $ sudo pip install flask-socketio
```
