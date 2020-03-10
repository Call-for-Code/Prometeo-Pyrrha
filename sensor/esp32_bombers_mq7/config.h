#ifndef __CONFIG_H__
#define __CONFIG_H__

//-------- Customise these values for each device-----------
const	char*		ssid = ""; 				//SSID of your WiFi Access Point
const	char*		password = ""; 			//Password of your WiFi Access Point
#define DHTTYPE		XXXXX 					//The DHT type of sensor you're using. Should work with DHT11 and DHT22
#define DHTPIN		NN 						//GPIO PIN fot DHT
#define analogMQ7	NN 						//GPIO PIN fot MQ
#define TIMEOUT 	5000 					//This timeout is for the DHT readings, you can increase it, but never decrease or the DHT readings will fail :(
#define DEVICE_TYPE "XXXXXXXXXX"			//This must match the type of device defined on your IBM IoT Platform
#define DEVICE_ID 	"XXXXXXXXXXX" 			//This must mach the DEVICE ID that you defined on you IBM IoT Platform
#define TOKEN 		"XXXXXXXXXXXXXXXXXXXX"	//The token taht you define, or the IBM IoT platform generated for your device
#define SLEEP_WAIT 	30 						//number of seconds that ESP32 will be doing nothing between readings
#define	ORG			"xxxxxx"  				//Customise this according your IBM Organization

//-------- Customise the above values according your timezone, it's important to have it configured properly as we need it for ssl communications--------
#define TZ_OFFSET 	N 						//Your timezone offset IE '-1' or '2'
#define TZ_DST 		NN  					//Daylight offset, the number of minutes that changes on summer time
//-------- Customise the above values according your timezone, it's important to have it configured properly as we need it for ssl communications--------
//-------- Customise these values for each device-----------

#endif
