#include <WiFi.h>
#include <WiFiClientSecure.h> 
#include <PubSubClient.h>
#include <DHT.h>
#include <time.h>
#include "esp_sleep.h"


//-------- Customise these values -----------
const char* ssid = "xxxxx";
const char* password = "xxxxx";
#define DHTTYPE DHT11
#define DHTPIN  27
#define MQ2pin (0)
#define NUM_FIELDS 3                // To update more fields, increase this number and add a field label below.
#define TIMEOUT 5000
#define ORG "o02qp9"
#define DEVICE_TYPE "FireFighter"
#define DEVICE_ID "0001"
#define TOKEN "xxxxxxxx"

#define TZ_OFFSET -1
#define TZ_DST 60
//-------- Customise the above values --------

char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char TopicSub[] = "iot-2/cmd/status/fmt/json";
char TopicPub[] = "iot-2/evt/status/fmt/json"; 
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

DHT dht(DHTPIN, DHTTYPE); 
WiFiClientSecure wifiClient;

PubSubClient client(server, 8883, wifiClient); //Never modify the 8883 as it is a safe port for sending data


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
 // esp_sleep_enable_timer_wakeup(50000000); //Wake up in 50 seconds
  esp_sleep_enable_timer_wakeup(5000000); //Wake up in 5 seconds
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
  configTime(TZ_OFFSET * 3600, TZ_DST * 60, "pool.ntp.org", "0.pool.ntp.org");
  Serial.println("\nWaiting for time");
  //wifiClient.setCACert(ca_cert);
  client.setCallback(callback); // Here we "connect"the callback function to subscribe data receive
}
