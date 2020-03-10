# Prometeo


In front of natural disasters, such as Wildfires, PROMETEO tries to protect those who protect us, the firefighters.

With our solution, we've developed a prototype sensor which sends basic telemetry (temperature, humidity and smoke concentration). These data is processed by a machine learning algorithm which is able to predict if the health of the firefighter is ok, will be in danger or if it's in danger.

So, let us explain how we achieved this through this readme.

[Project website](https://github.com/joraco-dev/prometeo)

[![](http://img.youtube.com/vi/vOgCOoy_Bx0/0.jpg)](http://www.youtube.com/watch?v=vOgCOoy_Bx0 "")


## The solution at a glance

![alt text](https://github.com/joraco-dev/prometeo/blob/master/content/Presentation2.png)

As you can see, our project has 5 major blocks, The Sensor, IBM IoT Hub, NodeRed, Container service and the client. Lets review them one by one.

### Pre-requisites

This project, at least the part of the board, has been developed with Arudino, so you will need the IDE if you want to use our example, you can get it here (https://www.arduino.cc/en/main/software).

Once it’s installed the Arudino IDE, you should install third party libraries for the ESP8266 board (the one that we use).

In order to do that, you should add this repository as follows. Go to preferences, then add on the field “Additional Boards Manager URLs” this URL https://arduino.esp8266.com/stable/package_esp8266com_index.json, if you already have some other repositories, separate them with a comma.

Next thing to do, is add our board. We need to click on Tools Menu, then on Board section, click on “Boards Manager”. We will search esp8266 and install the board called “esp8266 by ESP8266 Community” (at the moment of writing this manual, the selected version is 2.5.0).

The last thing before getting to the real work is install the libraries used by our project, we use libraries for the temperature sensor, for forming a JSON in order to send the data to the IBM IoT Cloud, we will need to install the following libraries.

We go to Sketch menu, then we will select “Include library” and finally “Manage Libraries”, then we will search and install the following libraries

- Adafruit ESP8266 by Adafruit 1.0.0
- ArduinoJson by Benoit Blanchon 5.13.5 (IMPORTANT!, the code need this specific version, as the upper version change the way it acces the information, from array to pointer, and will not compile if you don’t use this specific version)
- DHT sensor library by Adafruit 1.3.4
- EspMQTTClient by Patrick Lapointe 1.5.0

Once all this pre-requisites are met, you can start compiling the code

### The Sensor

For this part of the project, we used a board based on the ESP8266 micro-controller with NodeMCU firmware, as this project will be open source. To this controller, we attached a couple of sensors, the DHT11 which give us information about Temperature and Humidity and the MQ-2 sensor which give us, among the concentration of several gases, the concentration of smoke.

In order to do it portable, we decided to power the controller with a portable usb battery and wrap-it with a sports arm band.

The following image is the final result with a firefighter wearing it.

![alt text](https://github.com/joraco-dev/prometeo/blob/master/content/sensor.png)

Finally, the code that makes possible to read those metrics and send it to the next stage (IBM IoT Hub) [could be reviewed here](https://github.com/joraco-dev/prometeo/blob/master/sensor/sensor.c). There are some things to have in mind reading this code:
	
- The connectivity to the internet is intended to do it with tethering with a mobile phone, so you need to supply SSID and password of the wifi-hotspot.
- You need to take into consideration that the pins will vary if you decide to us another board or type of sensors, in our case for the Temperature and Humidity we use de the digital pin 5 and for the smoke sensor we used the analog pin 0.
- You will need a valid token for the IoT Hub and register your device in the Hub
- We based our code on the example provided on the IBM Developer portal written by Ant Elder (https://developer.ibm.com/recipes/tutorials/run-an-esp8266arduino-as-a-iot-foundation-managed-device/)
	

### IBM IoT HUB

This was very straight forward step, simply register the new devices and make sure they connect to the platform, also and thanks to the code uploaded to our microcontroller we're able to perform remote actions on our device, such as change the polling interval, restart the device or wipe it.

At last but not at least we created a connection between the IoT Hub and our next step, our NodeRed app.


### NodeRed

![alt text](https://github.com/joraco-dev/prometeo/blob/master/content/nodered2.png)

At this point, we can conclude that we're in front of our core service. With this app we control all the workflow of the metrics sent by our sensors, store them, analyze them and take actions depending on the readings.

So, lets analyse node by node. Also, you can find [the code here](https://github.com/joraco-dev/prometeo/blob/master/nodered/node.yml), if you want to import to your personal project, just take into account that credentials, tokens and sensitive data are deleted.

- get IAM Token: At this block, basically we generate every 15 minutes a request in order to have a valid token to authenitcate with the IAM service. We will use it in order to communicate with our Machine Learnin Webservice.

- IBM IoT: It connects and receives the events from every device registered in our IoT Hub. The messages are received in json format.
	
	Once the message is received we take two actions in parallel
- nodemcu: With this node, we save a copy of the message in a cloudant database, with this we will have historical data for the future.
- IoT2ML: At this function node, we only transform the message received in order to make it comprehensive by our machine learning service.
	
- Machine Learning Firefighter Health Prediction: in the Watson Machine Learning is where the "magic" happens, thanks to our predictive model, once we send the metrics, our model will reply with a Green, yellow or red firefighter status. We will go deeper on our explanation on the Watson Machine Learning section below.

- ML2status2.0: At this point, we finally prepare the message in order to be sent to our live dashboard. Basically we send the following paylod, "Firefighter ID", "Status", "Timestamp of the event", "Temperature", "Humidity", "Smoke concentration"

- Webscokets Server: This is the end node, which sends the messages to our websockets send and receive server, later we will talk more in detail.


### IBM Cloud Container Service

At this point, we need somewhere to publish our real time dashboard. We created a service at the IBM Cloud Container Service. This service includes a web-sockets receiver and sender and a NGINX serving our portal written basically with javascript and using datatables library based on jquery.

This service has exposed two ports, one for the web-sockets server and the other for the nginx server.

[Inside this folder](https://github.com/joraco-dev/prometeo/tree/master/server), you can find the code of the websockets server (server.js), the source for the portal (html/index.html) and also the script we use to deploy and update the code on our POD (deploy.sh).

### Client

Finally, the client is just a web browser that supports javascript and web-sockets that is support by almos all new browsers.

You can access to our Live dashboard [at this http address](http://169.51.194.198:32134/index.html) , unfortunately, if there is no sensors transmitting data, you will not be able to see it in action.

![alt text](https://github.com/joraco-dev/prometeo/blob/master/content/portal.png)



## Watson Machine Learning

As part of the Prometeo Solution, is used machine learning in order to determine if it is risky to maintain the firefighter in the actual area puting out the fire or if it is necessary to move him away.

We didn't have real data for training the model, so we decided to emulate data creating an ad hoc dataset.

First, we used the dataset to train the model.

After training the model, we created a web service in order to use the model in the NodeRed module explained before.


### Training the model

We used the dataset dataset_cut.csv [the code here](https://github.com/joraco-dev/prometeo/blob/master/content/dataset_cut.csv) that contains the next variables:

- Temperature: exterior temperature
- Humidity: Relative humidity in %
- PPM: Smoke concentration referred to parts of million of CO
- Status: indicates if the actual observation is red (meaning it's dangerous for the firefigheter), yellow (it's a warning), green (there's no risk). This is the variable we have to predict

The machine model we'll be trained with this information.

Before starting the training, it's necessary to process the dataset.

Once the dataset is uploaded in the watson studio project, refine the dataset with the next actions:
- Convert temperature, humidity and ppm into integer
- Execute the job creating a new version of the dataset

After that, create a new machine learning model:
- Use the dataset we have just created
- Use manual or automatic model creation
- Select a machine learning service, if it doesn't exist create a new one
- Model type must be "model builder"
- Select a Spark Scala Kernel, if it doesn't exist create a new one

Important you select "Status" as the variable you are going to predict. The rest of variables must be selected as feature variables. You have to select Multiclass Classification because we have 3 possible predictions (red, yellow and green).

Finally, train and evaluate the model.


![alt text](https://github.com/joraco-dev/prometeo/blob/master/content/Model_training.png)


### Using the model

Once the model is created, you can use it creating a web service.

In the trained model, select the Deployments tab and push the Add Deployments option. Name the new deployment and create it.

After creating the deployment, select it. You can test it, in the servie you have all the information about how to use it.

In our case, we created the deployment "Prometeo ML Webservice".

![alt text](https://github.com/joraco-dev/prometeo/blob/master/content/Model_deployment.png)

### Graphical explantation of our model
![alt text](https://github.com/joraco-dev/prometeo/blob/master/content/datascience.png)


## Authors

* **Josep Ràfols**
* **Marco Emilio Rodríguez**
* **Salomé Valero**
* **Joan Herrera**
* **Vicenç Ferrés**


## License

This project is licensed under the Apache 2 License

## Acknowledgments
* Juanma García Escobar ( www.juanma.es) for his original drawings and animations
* Artur Alvarez Herrera (www.artvr.tv) for his video production
* Michael Martinez  (michael.martineztx@gmail.com) for his speaking with american accent for the video
* Wildland Firefighters GRAF for their labor against wildfires in Catalonia and their inspiring message
* GRAF: Grups de Reforç d'Actuacions Forestals. Bombers. Departament Interior de la Generalitat de Catalunya. This technical support team is part of the local emergency services.  GRAF is specialized in fire extinction strategies.

* GEM: Grup d'Emergències Mèdiques dels Bombers de la Generalitat.
* Based on [Billie Thompson's README template](https://gist.github.com/PurpleBooth/109311bb0361f32d87a2).

