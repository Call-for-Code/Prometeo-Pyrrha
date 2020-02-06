# Setup

## Pre-requisites

This project, at least the hardware portion, has been developed with Arduino, so you will need the IDE if you want to use our example, you can get it here (https://www.arduino.cc/en/main/software).

Once the Arudino IDE is installed, you should install third party libraries for the ESP32 board (the specific one that we use).

In order to do that, you should add this repository as follows. Go to preferences, then add on the field “Additional Boards Manager URLs” this URL `https://dl.espressif.com/dl/package_esp32_index.json`, if you already have some other repositories, separate them with a comma.

Next thing to do is add our board to the IDE. We need to click on Tools Menu, then on Board section, click on “Boards Manager”. We will search `esp32` and install the board called “esp32 by Espressif Systems” (at the moment of writing this manual, the selected version is 1.0.4).

The last thing before getting to the real work is install the libraries used by our project. These include code for the temperature sensor and for forming a JSON document in order to send the data to the IBM Watson IoT Cloud.

We go to Sketch menu, then we will select “Include library” and finally “Manage Libraries”, then we will search and install the following libraries

- ArduinoJson by Benoit Blanchon 5.13.5 (IMPORTANT!, the code need this specific version, as the upper version change the way it acces the information, from array to pointer, and will not compile if you don’t use this specific version)
- DHT sensor library by Adafruit 1.3.4
- EspMQTTClient by Patrick Lapointe 1.5.0

Once all these pre-requisites are met, you can start compiling the code.

## The Sensor

For this part of the project, we used a board based on the ESP32 micro-controller with NodeMCU firmware, as this project will be open source. To this controller, we attached a couple of sensors, the DHT11 which gives us information about Temperature and Humidity and the MQ-7 sensor which gives us, among the concentration of several gases, the concentration of smoke and carbon monoxide.

In order for it to be wireless, we decided to power the controller with a portable USB battery and wrap it with a sports arm band.

The following image is the final result of the initial prototype with a firefighter wearing it.

![alt text](/screenshots/sensor.png)

Finally, the code that makes it possible to read those metrics and send them to the server (IBM IoT Platform) [can be reviewed here](/sensor/esp32_bombers_mq7/esp32_bombers_mq7.ino). There are some things to keep in mind reading this code:
	
- The connectivity to the internet is provided by tethering with a mobile phone, so you need to supply SSID and password of the WiFi-hotspot.
- You need to take into consideration that the pins will vary if you decide to us another board or type of sensor, in our case for the Temperature and Humidity we use de the digital pin 27 and for the smoke sensor we used the analog pin 32.
- You will need a valid token for the IoT Platform and to register your device in the Hub.
- We based our code on the example provided on the IBM Developer portal written by Ant Elder (https://developer.ibm.com/recipes/tutorials/run-an-esp8266arduino-as-a-iot-foundation-managed-device/)

Wiring diagram:

```
                                                               
                                                                +--------------------------+
                                             ESP32              |                          |
                                      +-----------------+       |           MQ7            |
          DHT11                       |              GND+-+     |  +-----------------+     |
+----------------------+  +-----------+3V3              | |     |  |                 |     |
|                      |  |           |    +-------+    | |     +--+VCC              |     |
|  +---------+     VCC +--+           |    | ESP32 |    | |        |       OoooO     |     |
|  |---------|         |              |    |       |    | +--------+GND   OoooooO    |     |
|  |---------|     DAT +----------+   |    |       |    |          |      OoooooO    |     |
|  |---------|         |          |   |    | WIFI  |    |          |D0    OoooooO    |     |
|  +---------+     GND +--+       |   |    +-------+    |          |       OoooO     |     |
|                      |  |       |   |                 |       +--+A0               |     |
+----------------------+  |       |   |                 |       |  |                 |     |
                          |       +---+G27              |       |  +-----------------+     |
                          |           |              G32+-------+                          |
                          |           |                 |                                  |
                          |           |                 |                                  |
                          +-----------+GND              |                                  |
                                 +----+5V               |                                  |
                                 |    |                 |                                  |
                                 |    +------_USB_------+                                  |
                                 |                                                         |
                                 +---------------------------------------------------------+

```
	

## IBM IoT Platform

This is a very straight forward step, simply register the new devices and make sure they connect to the platform, also and thanks to the code uploaded to our microcontroller we're able to perform remote actions on our device, such as change the polling interval, restart the device, or wipe it.

At last but not at least we create a connection between the IoT Platform and our next step, our Node-RED app.


## Node-RED

![alt text](/screenshots/nodered2.png)

At this point, we can conclude that we're in front of our core service. With this app we control all the workflow of the metrics sent by our sensors, store them, analyze them and take actions depending on the readings.

So, lets analyze node by node. Also, you can find [the code here](/nodered/node.yml), if you want to import to your personal project, just take into account that credentials, tokens and sensitive data are deleted.

- get IAM Token: At this block, basically we generate every 15 minutes a request in order to have a valid token to authenticate with the IAM service. We will use it in order to communicate with our machine learning service.

- IBM IoT: It connects and receives the events from every device registered in our IoT Hub. The messages are received in JSON format.
	
	Once the message is received we take two actions in parallel:
- nodemcu: With this node, we save a copy of the message in a Cloudant database, with this we will have historical data for the future.
- IoT2ML: At this function node, we only transform the message received in order to make it comprehensive by our machine learning service.
	
- Machine Learning Firefighter Health Prediction: in the Watson Machine Learning is where the "magic" happens, thanks to our predictive model, once we send the metrics, our model will reply with a green, yellow or red firefighter status. We will go deeper on our explanation on the Watson machine learning section below.

- ML2status2.0: At this point, we finally prepare the message in order to be sent to our live dashboard. Basically we send the following paylod, "Firefighter ID", "Status", "Timestamp of the event", "Temperature", "Humidity", "Smoke concentration"

- Websockets Server: This is the end node, which sends the messages to our websockets send and receive server, later we will describe it more in detail.


## IBM Cloud Kubernetes Service

At this point, we need somewhere to publish our real-time dashboard. We created a service at the IBM Cloud Container Service. This service includes a websockets receiver and sender and a NGINX hosting our portal written basically with Javascript and using datatables library based on jQuery.

This service has exposed two ports, one for the websockets server and the other for the NGINX server.

[Inside this folder](/dashboard/server), you can find the code of the websockets server `server.js`, the source for the portal `html/index.html` and also the script we use to deploy and update the code on our pod `deploy.sh`.

## Dashboard

Finally, the client is just a web browser that supports JavaScript and websockets that is support by almost all recent browsers.

![alt text](/screenshots/portal.png)

## Watson Machine Learning

Prometeo solution uses machine learning in order to determine the level of risk to keep the firefighter in the area putting out the fire or if it is imperative to move him away.

We didn't have real data for training the model, so we decided to emulate data creating an ad hoc dataset.

First, we used the dataset to train the model.

After training the model, we created a web service in order to use the model in the Node/Red module explained above.

## Training the model

We used the dataset [`dataset_cut.csv`](/content/dataset_cut.csv) that contains the next variables:

- Temperature: Exterior temperature
- Humidity: Relative humidity percentage
- PPM: Smoke concentration referred to parts of million of carbon monoxide (CO)
- Status: Indicates if the actual observation is red (meaning it's dangerous for the firefighter), yellow (it's a warning), green (there's low risk). This is the variable we have to predict.

The machine model we'll be trained with this information.

Before starting the training, it's necessary to process the dataset.

Once the dataset is uploaded in the Watson Studio project, refine the dataset with the next actions:
- Convert temperature, humidity, and PPM into integers
- Execute the job creating a new version of the dataset

After that, create a new machine learning model:
- Use the dataset we have just created
- Use manual or automatic model creation
- Select a machine learning service, if it doesn't exist create a new one
- Model type must be "model builder"
- Select a Spark Scala Kernel, if it doesn't exist create a new one

Important you select "Status" as the variable you are going to predict. The rest of variables must be selected as feature variables. You have to select Multiclass Classification because we have 3 possible predictions (red, yellow and green).

Finally, train and evaluate the model.

![alt text](/screenshots/Model_training.png)


## Using the model

Once the model is created, you can use it creating a web service.

In the trained model, select the Deployments tab and push the Add Deployments option. Name the new deployment and create it.

After creating the deployment, select it. You can test it, in the servie you have all the information about how to use it.

In our case, we created the deployment "Prometeo ML Webservice".

![alt text](/screenshots/Model_deployment.png)

## Graphical explantation of our model

![alt text](/screenshots/datascience.png)
