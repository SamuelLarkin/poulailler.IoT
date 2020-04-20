// [ESP8266 DS18B20 Temperature Sensor with Arduino IDE (Single, Multiple, Web Server)](https://randomnerdtutorials.com/esp8266-ds18b20-temperature-sensor-web-server-with-arduino-ide/)
// [Arduino the Object Oriented way](http://paulmurraycbr.github.io/ArduinoTheOOWay.html)
// [Multicast Domain Name System](https://tttapa.github.io/ESP8266/Chap08%20-%20mDNS.html)

// Import required libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>        // Include the mDNS library
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "html.h"
#include "credential.h"


// Replace with your network credentials
/*
const char* ssid="SSID";
const char* password="SSID_PASSWORD";
const char* mqtt_server = "192.168.XX.YY";
*/


#define DHTPIN  5       // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE);

// current temperature & humidity, updated in loop()
float DHT22_temperature = -100.0;
float DHT22_humidity = -100.0;


// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

float DS18B20_temperature = -100.0;


// Create AsyncWebServer object on port 80
AsyncWebServer web_server(80);

WiFiClient espClient;
PubSubClient mqtt_client(espClient);


// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;    // will store last time DHT was updated

// Updates DHT readings every 10 seconds
const long interval = 10000;  




// Replaces placeholder with DHT values
// The async web server uses template/ %variable% that can be replaced programatically.
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return String(DHT22_temperature);
  }
  else if(var == "HUMIDITY"){
    return String(DHT22_humidity);
  }
  else if(var == "DTEMPERATURE"){
    return String(DS18B20_temperature);
  }
  return String();
}



void setup_mDNS() {
   if (!MDNS.begin("poulailler")) {             // Start the mDNS responder for esp8266.local
      Serial.println("Error setting up MDNS responder!");
   }
   else {
      Serial.println("mDNS responder started");
   }
}



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

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  setup_mDNS();
}



void setup_web_server(AsyncWebServer& web_server) {
  // Route for root / web page
  web_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  web_server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(DHT22_temperature).c_str());
  });

  web_server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(DHT22_humidity).c_str());
  });

  web_server.on("/DS18B20/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(DS18B20_temperature).c_str());
  });

  // Start server
  web_server.begin();
}



void setup_serial() {
  // Serial port for debugging purposes
  Serial.begin(115200);
    // wait for serial monitor to open
  while(! Serial);
}



void setup_sensors(DHT& dht, DallasTemperature& sensors) {
  dht.begin();

  // Start the DS18B20 sensor
  sensors.begin();
}



void reconnect_mqtt() {
  // Loop until we're reconnected
  while (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (mqtt_client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //mqtt_client.publish("poulailler", "hello world");
      // ... and resubscribe
      //mqtt_client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



 
void setup(){
  setup_serial();
  setup_sensors(dht, sensors);
  setup_wifi();
  setup_web_server(web_server);

  mqtt_client.setServer(mqtt_server, 1883);
}



void loop(){  
  if (!mqtt_client.connected()) {
    reconnect_mqtt();
  }
  mqtt_client.loop();
  
  const unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you updated the DHT values
    previousMillis = currentMillis;
    
    // Read temperature as Celsius (the default)
    const float newT = dht.readTemperature();
    // if temperature read failed, don't change t value
    if (isnan(newT)) {
      Serial.println("Failed to read from DHT sensor!");
      DHT22_temperature = -200.0;
    }
    else {
      DHT22_temperature = newT;
      Serial.print(String(currentMillis) + " - ");
      Serial.print("DHT22: ");
      Serial.print(DHT22_temperature);
      Serial.println("°C");
    }

    // Read Humidity
    const float newH = dht.readHumidity();
    // if humidity read failed, don't change h value 
    if (isnan(newH)) {
      Serial.println("Failed to read from DHT sensor!");
      DHT22_humidity = -200.0;
    }
    else {
      DHT22_humidity = newH;
      Serial.print(String(currentMillis) + " - ");
      Serial.print("DHT22: ");
      Serial.print(DHT22_humidity);
      Serial.println("%H");
    }

    //mqtt_client.publish("poulailler/temperature", String(DHT22_temperature).c_str());
    //mqtt_client.publish("poulailler/humidite", String(DHT22_humidity).c_str());
    mqtt_client.publish("poulailler/sensor_1", (String("{\"temperature\":") + String(DHT22_temperature) + String(",\"humidite\":") + String(DHT22_humidity) + String("}")).c_str());

    sensors.requestTemperatures(); 
    DS18B20_temperature = sensors.getTempCByIndex(0);
    Serial.print(String(currentMillis) + " - ");
    Serial.print("DS18B20: ");
    Serial.print(DS18B20_temperature);
    Serial.println("°C");
    //mqtt_client.publish("poulailler/sensor_2", (String("{\"temperature\":") + String(DS18B20_temperature) + String(",\"humidite\": -100}")).c_str());
    mqtt_client.publish("poulailler/sensor_2", (String("{\"temperature\":") + String(DS18B20_temperature) + String("}")).c_str());
  }
}
