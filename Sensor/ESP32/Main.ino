#include <WiFi.h>
#include <WiFiClient.h> 
#include <PubSubClient.h>
#include <DHT.h>
#include "esp_sleep.h"


//-------- Customise these values -----------
const char* ssid = "jorawei";
const char* password = "joraco84";
#define DHTTYPE DHT11
#define DHTPIN  27
#define MQ2pin (0)
#define NUM_FIELDS 3                               // To update more fields, increase this number and add a field label below.
#define TEMPER 0                      // ThingSpeak field for soil moisture measurement.
#define HUMEDAD 1                       // ThingSpeak Field for elapsed time from startup.
#define FUM 2                       // ThingSpeak Field for elapsed time from startup.
#define TIMEOUT 5000
#define ORG "o02qp9"
#define DEVICE_TYPE "FireFighter"
#define DEVICE_ID "0001"
#define TOKEN "pUhG-qdPhfg-BR@en*"
//-------- Customise the above values --------

char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char TopicSub[] = "iot-2/cmd/status/fmt/json";
char TopicPub[] = "iot-2/evt/status/fmt/json"; 
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
//unsigned int Delay = 30;    // This time is what the device will take to send data
//unsigned int i=(Delay*100);

DHT dht(DHTPIN, DHTTYPE); 
WiFiClient wifiClient;
PubSubClient client(server, 1883, wifiClient); //Never modify the 8883 as it is a safe port for sending data


void callback(char* topic, byte* payload, unsigned int length) 
{
  String data="";
  for (int i = 0; i < length; i++) 
  {
    data+=char(payload[i]);
  }
  Serial.println("Received Data:" + data); // In this case we print the data recive from the website.
}

void setup() {
  Serial.begin(9600); Serial.println();
  dht.begin();           // initialize temperature sensor
  esp_sleep_enable_timer_wakeup(50000000); //Wake up in 50 seconds
  wifiConnect();
}

void loop() {
  client.loop();
   
   // Do not modify the delay of 500 ms since it depends on the correct connection.
   if (!!!client.connected()) 
   {
     Serial.print("Reconnecting client to "); Serial.println(server);
     while ( !client.connect(clientId, authMethod, token)) 
     {
        Serial.print(".");
        delay(500);
     }
     Serial.println();
     client.subscribe(TopicSub);  // This is for callback
   }

    String payload = "{\"Temperature\":";
    payload += dht.readTemperature();
    payload += ", \"Humidity\":";
    payload += dht.readHumidity();
    payload += ", \"Smoke\":";
    payload += analogRead(MQ2pin);
    payload += "}";

    Serial.print("Sending payload: "); Serial.println(payload);
      
    if (client.publish(TopicPub, (char*) payload.c_str())) 
    {
      Serial.println("Publish ok");
    } 
    else 
    {
      Serial.println("Publish failed");
    }
    
    delay(5000); //giving time to send the whole request
    Serial.println( "Going for a siesta!" );
    esp_deep_sleep_start();

}

void wifiConnect() {
  Serial.print("Connecting to "); Serial.print(ssid);
  if (strcmp (WiFi.SSID().c_str(), ssid) != 0) {
     WiFi.begin(ssid, password);
  }
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }  
  Serial.println("");
  Serial.print("WiFi connected, IP address: "); Serial.println(WiFi.localIP());
  client.setCallback(callback); // Here we "connect"the callback function to subscribe data receive
}
