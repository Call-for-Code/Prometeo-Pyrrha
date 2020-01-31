#ifndef __CONFIG_H__
#define __CONFIG_H__

// Network
const char* ssid = "enter-wifi-ssid-here";
const char* password = "enter-wifi-pass-here";

// Device
#define DHTTYPE DHT11
#define DHTPIN  27
#define analogMQ7 32
#define TIMEOUT 5000
#define ORG "enter-org-here"
#define DEVICE_TYPE "FireFighter"
#define DEVICE_ID "0001"
#define TOKEN "enter-token-here"
#define TZ_OFFSET -1
#define TZ_DST 60
const int AOUTpin = 32;

// Watson IoT
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char TopicSub[] = "iot-2/cmd/status/fmt/json";
char TopicPub[] = "iot-2/evt/status/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

#endif