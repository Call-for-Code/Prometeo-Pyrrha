#include <WiFi.h>
#include <WiFiClientSecure.h> 
#include <PubSubClient.h>
#include <DHT.h>
#include <time.h>
#include "MQ7.h" //https://github.com/swatish17/MQ7-Library
#include "esp_sleep.h" 
#include "config.h" //this file MUST be at the same folder as the .ino file

// --- Define subscription model. No need to modify
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char TopicSub[] = "iot-2/cmd/status/fmt/json";
char TopicPub[] = "iot-2/evt/status/fmt/json"; 
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
// --- Define subscription model. No need to modify

DHT dht(DHTPIN, DHTTYPE); //Setting up the DHT Sensor
MQ7 mq7(analogMQ7,5.0); //Setting up to 5V the MQ Sensor
WiFiClientSecure wifiClient; //Setting up wifi

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
  dht.begin();           // initialize temperature/humidity sensor
  wifiConnect();         //Connecting to wifi for the first time
  esp_sleep_enable_timer_wakeup(SLEEP_WAIT * 1000000); //seconds * uSeconds; 
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
     client.subscribe(TopicSub);  // This is for callback

   }

   //Build the payload in order to be sent to IBM IoT Platform
   String payload = "{\"Temperature\":";
   payload += dht.readTemperature();
   payload += ", \"Humidity\":";
   payload += dht.readHumidity();
   payload += ", \"CO\":";
   payload += mq7.getPPM();
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
   esp_deep_sleep_start(); //We will activate deep sleep mode, and it will remain sleeping the number of seconds that we set up at esp_sleep_enable_timer_wakeup function.

}

void wifiConnect() {
  int timeout = 0; //Timeout variable to control wifi reconnection.
  
  Serial.print("Connecting to "); Serial.print(ssid);
  if (strcmp (WiFi.SSID().c_str(), ssid) != 0) {
     WiFi.begin(ssid, password);
  }
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     timeout++;
     if (timeout > 20 ) //Sometimes the reconnection of the WiFi doesn't work properly, so if it tries N times without success, we reset the device. All the times this works fine.
     {
        Serial.print("WiFi not recconecting, reset!");
        ESP.restart();
     }
     Serial.print("W");
  }  
  Serial.println("");
  Serial.print("WiFi connected, IP address: "); Serial.println(WiFi.localIP());
  configTime(TZ_OFFSET * 3600, TZ_DST * 60, "pool.ntp.org", "0.pool.ntp.org");
  Serial.println("\nWaiting for time");
  //wifiClient.setCACert(ca_cert);
  client.setCallback(callback); // Here we "connect" the callback function to subscribe data receive
}
