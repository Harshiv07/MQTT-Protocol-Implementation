#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>                                                    // esp8266 library
#include <FirebaseArduino.h>                                                // firebase library
#include <DHT.h>

// Uncomment one of the lines bellow for whatever DHT sensor type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// DHT Sensor
#define DHTPin D5

// Change the credentials below, so your ESP8266 connects to your router
//const char* ssid = "iBall-Baton";
//const char* password = "chalbhaagne";

//const char* ssid = "linksys";
//const char* password = "cnproject";

//const char* ssid = "Bro";
//const char* password = "qwerty12345";

const char* ssid = "Redmi1";
const char* password = "jineshpatel";


// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "192.168.43.134";
// Initializes the espClient
WiFiClient espClient;

#define FIREBASE_HOST "cn-project-4301f.firebaseio.com"                          // the project name address from firebase id
#define FIREBASE_AUTH "higCHz09mwA9Dq5juCuvIKJGzWfFJPJE8j2cFy9y"            // the secret key generated from firebase


PubSubClient client(espClient);
// Connect an LED to each GPIO of your ESP8266

const int ledGPIO5 = 5;
const int ledGPIO4 = 4;
const int ledPin = 2;
int brightness;
String bright = "";
String ppm;
double d;


// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

// Timers auxiliar variables
long now = millis();
long lastMeasure = 0;

// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that
// your ESP8266 is subscribed you can actually do something
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic home/office/esp1/gpio2, you check if the message is either 1 or 0. Turns the ESP GPIO according to the message

  if(topic=="esp8266/4"){
      Serial.print("Changing GPIO 4 to ");
      if(messageTemp == "1client1"){
        digitalWrite(ledGPIO4, LOW);
        Serial.print("On");
      }
      else if(messageTemp == "0client1"){
        digitalWrite(ledGPIO4, HIGH);
        Serial.print("Off");
      }
      /*else if(messageTemp == ""){
        Serial.print("I got the value");
      }*/
  }
  if(topic=="esp8266/5"){
      Serial.print("Changing GPIO 5 to ");
      if(messageTemp == "1client1"){
        digitalWrite(ledGPIO5, LOW);
        Serial.print("On");
      }
      else if(messageTemp == "0client1"){
        digitalWrite(ledGPIO5, HIGH);
        Serial.print("Off");
      }
  }

  if(topic=="submit"){
    Serial.print("We got into submit  ");
    Serial.println(messageTemp);
    if(messageTemp!=0){
    brightness = (messageTemp.toInt()*1024)/100;
    //if(messageTemp != "")
    analogWrite(ledPin,brightness);
    }
    else
    analogWrite(ledPin,LOW);

  }
  Serial.println();
}

// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
     /*
     YOU  NEED TO CHANGE THIS NEXT LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
     To change the ESP device ID, you will have to give a unique name to the ESP8266.
     Here's how it looks like now:
       if (client.connect("ESP8266Client")) {
     If you want more devices connected to the MQTT broker, you can do it like this:
       if (client.connect("ESPOffice")) {
     Then, for the other ESP:
       if (client.connect("ESPGarage")) {
      That should solve your MQTT multiple connections problem

     THE SECTION IN loop() function should match your device name
    */
    if (client.connect("ESP8266Client1")) {
      Serial.println("connected");
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe("esp8266/4");
      client.subscribe("esp8266/5");
      client.subscribe("submit");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// The setup function sets your ESP GPIOs to Outputs, starts the serial communication at a baud rate of 115200
// Sets your mqtt broker and sets the callback function
// The callback function is what receives messages and actually controls the LEDs
void setup() {
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                              // connect to firebase
  dht.begin();
  pinMode(ledGPIO4, OUTPUT);
  pinMode(ledGPIO5, OUTPUT);
  digitalWrite(ledGPIO4, HIGH);
  digitalWrite(ledGPIO5, HIGH);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

// For this project, you don't need to change anything in the loop function.
// Basically it ensures that you ESP is connected to your broker
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
     /*
     YOU  NEED TO CHANGE THIS NEXT LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
     To change the ESP device ID, you will have to give a unique name to the ESP8266.
     Here's how it looks like now:
       client.connect("ESP8266Client");
     If you want more devices connected to the MQTT broker, you can do it like this:
       client.connect("ESPOffice");
     Then, for the other ESP:
       client.connect("ESPGarage");
      That should solve your MQTT multiple connections problem

     THE SECTION IN recionnect() function should match your device name
    */
    client.connect("ESP8266Client1");

  now = millis();
  // Publishes new temperature and humidity every 10 seconds
  if (now - lastMeasure > 1000) {
    lastMeasure = now;
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    //float f = dht.readTemperature(true);
    int sensorValue = analogRead(A0);   // read the input on analog pin 0
    if((Serial.available())>0){
      ppm = Serial.readStringUntil('\n');
      d = ppm.toDouble();
    }

  //float voltage = sensorValue * (5.0 / 1023.0);   // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V)

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Computes temperature values in Celsius
    //float hic = dht.computeHeatIndex(t, h, false);
    static char temperatureTemp[7];
    static char ldr1Temp[7];
    dtostrf(t, 6, 2, temperatureTemp);

    // Uncomment to compute temperature values in Fahrenheit
    // float hif = dht.computeHeatIndex(f, h);
    // static char temperatureTemp[7];
    // dtostrf(hic, 6, 2, temperatureTemp);

    static char humidityTemp[7];
    static char cotwo[7];
    dtostrf(h, 6, 2, humidityTemp);
    dtostrf(sensorValue, 6, 2, ldr1Temp);
    dtostrf(d, 6, 2, cotwo);
    int l = (sensorValue * 100) / 1023;
//    String fireHumid = String(h) + String("%");                                         //convert integer humidity to string humidity
//    String fireTemp = String(t) + String("°C");
//    String fireL = String(l) + String("%");
//    String fireC = String(d);

    String fireHumid = String(h);                                         //convert integer humidity to string humidity
    String fireTemp = String(t);
    String fireL = String(l);
    String fireC = String(d);

    // Publishes Temperature and Humidity values
    client.publish("/esp8266/temperature1", temperatureTemp);
    client.publish("/esp8266/humidity1", humidityTemp);
    client.publish("/esp8266/ldr1", ldr1Temp);
    client.publish("/esp8266/mq1", cotwo);


    Firebase.pushString("/nDHT11/HumidityPercent", fireHumid);                                  //setup path and send readings
    Firebase.pushString("/nDHT11/TemperatureDegreeCelsius", fireTemp);                                //setup path and send readings
    Firebase.pushString("/nDHT11/BrightnessPercent", fireL);
    Firebase.pushString("/nDHT11/CO2_PPM", fireC);
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    //Serial.print(f);
    //Serial.print(" *F\t Heat index: ");
    //Serial.print(hic);
    //Serial.println(" *C ");
    Serial.print("\nLDR : ");
    Serial.println(sensorValue);   // print out the value you read
    Serial.print("CO2 (in ppm) : ");
    Serial.println(ppm.toFloat());
    // Serial.print(hif);
    // Serial.println(" *F");
    ppm = (String)d;
  }
}
