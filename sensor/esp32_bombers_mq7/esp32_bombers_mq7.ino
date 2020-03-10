#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <time.h>
#include "MQ7.h"
#include "esp_sleep.h"

// This file MUST be in the same folder as the .ino file
#include "config.h"

// Define subscription model. No need to modify
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char TopicSub[] = "iot-2/cmd/status/fmt/json";
char TopicPub[] = "iot-2/evt/status/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

// Setting up the DHT Sensor
DHT dht(DHTPIN, DHTTYPE);

// Setting up to 5V the MQ Sensor
MQ7 mq7(analogMQ7,5.0);

// Setting up WiFi
WiFiClientSecure wifiClient;

// Never modify '8883', as it is a safe port for sending data
PubSubClient client(server, 8883, wifiClient);

void callback(char* topic, byte* payload, unsigned int length)
{
	String data="";
	for (int i = 0; i < length; i++)
	{
		data+=char(payload[i]);
	}
	// Print the data received from the website.
	Serial.println("Received Data:" + data);
}

void setup() {
	Serial.begin(9600);
	Serial.println();

	// Initialize temperature/humidity sensor
	dht.begin();

	// Connecting to WiFi for the first time
	wifiConnect();

	// seconds * uSeconds
	esp_sleep_enable_timer_wakeup(SLEEP_WAIT * 1000000);
}

void loop() {
	client.loop();

	if (!!!client.connected())
	{
		Serial.print("Reconnecting client to ");
		Serial.println(server);
		while (!client.connect(clientId, authMethod, token))
		{
			Serial.print(".");

			// Do not modify the delay of 500ms, as it depends on the correct connection.
			delay(500);
		}
		// This is for callback
		client.subscribe(TopicSub);
	}

	// Build the payload to send to the IBM IoT Platform
	String payload = "{\"Temperature\":";
	payload += dht.readTemperature();
	payload += ", \"Humidity\":";
	payload += dht.readHumidity();
	payload += ", \"CO\":";
	payload += mq7.getPPM();
	payload += "}";

	Serial.print("Sending payload: ");
	Serial.println(payload);

	if (client.publish(TopicPub, (char*) payload.c_str()))
	{
		Serial.println("Publish ok");
	}
	else
	{
		Serial.println("Publish failed");
	}

	// Give time to send the whole request
	delay(5000);
	Serial.println( "Going for a siesta!" );

  // Activate deep sleep mode - it will remain in sleep mode for the number of seconds that we setup in the esp_sleep_enable_timer_wakeup function.
  // Not compatible with iOS devices - comment out the following line to enable compatibility with iOS devices.
	esp_deep_sleep_start(); 
}

void wifiConnect() {
  // Timeout variable to control WiFi reconnection.
	int timeout = 0;

	Serial.print("Connecting to ");
  Serial.print(ssid);
  
	if (strcmp (WiFi.SSID().c_str(), ssid) != 0) {
		WiFi.begin(ssid, password);
	}
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		timeout++;

    // Sometimes reconnecting to WiFi doesn't work properly, so if it tries N times without success, reset the device.
		if (timeout > 20 )
		{
			Serial.print("WiFi not recconecting, reset!");
			ESP.restart();
		}
		Serial.print("W");
	}
	Serial.println("");
	Serial.print("WiFi connected, IP address: ");
  Serial.println(WiFi.localIP());
  
	configTime(TZ_OFFSET * 3600, TZ_DST * 60, "pool.ntp.org", "0.pool.ntp.org");
  
	Serial.println("\nWaiting for time");

  // "Connect" to the callback function to subscribe for data receiving
	client.setCallback(callback); 
}
