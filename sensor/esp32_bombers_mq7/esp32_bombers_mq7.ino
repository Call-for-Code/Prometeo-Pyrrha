#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <time.h>
#include "MQ7.h"
#include "esp_sleep.h"
#include "config.h"

DHT dht(DHTPIN, DHTTYPE);
MQ7 mq7(analogMQ7, 5.0);
WiFiClientSecure wifiClient;

// Never modify the 8883 as it is a safe port for sending data
PubSubClient client(server, 8883, wifiClient);

void callback(char* topic, byte* payload, unsigned int length)
{
  String data = "";
  for (int i = 0; i < length; i++)
  {
    data += char(payload[i]);
  }

  // In this case we print the data receive from the web site.
  Serial.println("Received Data:" + data);
}

void setup()
{
  Serial.begin(9600); Serial.println();

  // Initialize sensor
  dht.begin();

  wifiConnect();

  // Seconds * uSeconds
  esp_sleep_enable_timer_wakeup(5 * 1000000);
}

void loop()
{
  client.loop();

  // Do not modify the delay of 500 ms since it depends on the correct connection.
  if (!!!client.connected())
  {
    Serial.print("Reconnecting client to "); Serial.println(server);
    while (!client.connect(clientId, authMethod, token))
    {
      Serial.print(".");
      delay(500);
    }

    // Debug
    Serial.println("re-connected");

    // This is for callback
    client.subscribe(TopicSub);
  }

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

  // Giving time to send the whole request
  delay(5000);
  Serial.println("Going for a siesta!");

  // rtc_gpio_isolate(GPIO_NUM_17);
  esp_deep_sleep_start();
}

void wifiConnect()
{
  Serial.print("Connecting to "); Serial.print(ssid);

  if (strcmp (WiFi.SSID().c_str(), ssid) != 0)
  {
    WiFi.begin(ssid, password);
  }

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.print("WiFi connected, IP address: "); Serial.println(WiFi.localIP());

  configTime(TZ_OFFSET * 3600, TZ_DST * 60, "pool.ntp.org", "0.pool.ntp.org");
  Serial.println("\nWaiting for time");

  // wifiClient.setCACert(ca_cert);

  // Here we "connect" the callback function to subscribe data receive
  client.setCallback(callback);
}
