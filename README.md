# ![Prometeo](/screenshots/prometeo.png?raw=true)

[![License](https://img.shields.io/badge/License-Apache2-blue.svg)](https://www.apache.org/licenses/LICENSE-2.0) [![Slack](https://img.shields.io/static/v1?label=Slack&message=%23prometeo-pyrrha&color=blue)](https://callforcode.org/slack)

In the face of natural disasters, such as wildfires, Prometeo protects those who protect us - the firefighters. With our solution, we've developed a prototype sensor which sends environmental telemetry (temperature, humidity, and smoke concentration). This data is processed by a machine learning algorithm which is able to predict firefighter health, indicating if they are OK, soon to be in danger, or in danger. The goal is to track real-time status, and eventually use aggregated data to make suggestions that improve health outcomes in the long run.

> **Note:** The [Prometeo name is now used for the startup behind the open source project](https://prometeoplatform.com/). The project itself is now composed into separate repos under the [Pyrrha Platform organization](https://github.com/Pyrrha-Platform) which is the official Call for Code with the Linux Foundation project name:

* [Pyrrha](https://github.com/Pyrrha-Platform/Pyrrha)
* [Pyrrha-Dashboard](https://github.com/Pyrrha-Platform/Pyrrha-Dashboard)
* [Pyrrha-Database](https://github.com/Pyrrha-Platform/Pyrrha-Database)
* [Pyrrha-Pyrrha-Deployment-Configurations](https://github.com/Pyrrha-Platform/Pyrrha-Deployment-Configurations)
* [Pyrrha-Firmware](https://github.com/Pyrrha-Platform/Pyrrha-Firmware)
* [Pyrrha-Hardware](https://github.com/Pyrrha-Platform/Pyrrha-Hardware)
* [Pyrrha-Mobile-App](https://github.com/Pyrrha-Platform/Pyrrha-Mobile-App)
* [Pyrrha-MQTT-Client](https://github.com/Pyrrha-Platform/Pyrrha-MQTT-Client)
* [Pyrrha-Rules-Decision](https://github.com/Pyrrha-Platform/Pyrrha-Rules-Decision)
* [Pyrrha-Sensor-Simulator](https://github.com/Pyrrha-Platform/Pyrrha-Sensor-Simulator)
* [Pyrrha-Watch-App](https://github.com/Pyrrha-Platform/Pyrrha-Watch-App)
* [Pyrrha-Website](https://github.com/Pyrrha-Platform/Pyrrha-Website)
* [Pyrrha-WebSocket-Server](https://github.com/Pyrrha-Platform/Pyrrha-WebSocket-Server)

## The solution at a glance (DEPRECATED, SUPERSEDED BY PYRRHA ABOVE)

Prometeo is comprised of 5 different components:
1. [The sensor](SETUP.md#the-sensor)
2. [IBM IoT Platform](SETUP.md#ibm-iot-platform)
3. [Node-RED](SETUP.md#node-red)
4. [IBM Cloud Kubernetes Service](SETUP.md#ibm-cloud-kubernetes-service)
5. [Dashboard](SETUP.md#dashboard)

![Prometeo v1 architecture](https://github.com/joraco-dev/prometeo/blob/master/content/Presentation2.png)

## Get started

* [The Prometeo story](#the-prometeo-story)
* [Setting up the solution](#setting-up-the-solution)
* [Project Roadmap](#project-roadmap)
* [Built with](#built-with)
* [Contributing](#contributing)
* [Authors](#authors)
* [License](#license)
* [Acknowledgments](#acknowledgments)

## The Prometeo story
Watch the video below to understand Prometeo's solution:

[![](http://img.youtube.com/vi/vOgCOoy_Bx0/0.jpg)](http://www.youtube.com/watch?v=vOgCOoy_Bx0)

## Setting up the solution

See [SETUP.md](SETUP.md)

## Project roadmap

See [ROADMAP.md](ROADMAP.md)

## Built with

* [IBM IoT Platform](https://www.ibm.com/internet-of-things/solutions/iot-platform/watson-iot-platform) - Used to collect and capture data from the sensor
* [Node-RED](https://nodered.org/) - Used to run a custom machine learning algorithm on the collected data
* [IBM Cloud Kubernetes Service](https://www.ibm.com/cloud/container-service/) - Used to connect the data to the dashboard

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct, and the process for submitting Prometeo pull requests.

## Authors

* Josep Ràfols
* Marco Emilio Rodríguez
* Salomé Valero
* Joan Herrera
* Vicenç Ferrés

## License

This project is licensed under the Apache 2 License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments
* Juanma García Escobar (www.juanma.es) for his original drawings and animations
* Artur Alvarez Herrera (www.artvr.tv) for his video production
* Michael Martinez (michael.martineztx@gmail.com) for his speaking with American accent for the video
* Wildland Firefighters GRAF for their labor against wildfires in Catalonia and their inspiring message
* GRAF: Grups de Reforç d'Actuacions Forestals. Bombers. Departament Interior de la Generalitat de Catalunya. This technical support team is part of the local emergency services. GRAF is specialized in fire extinction strategies.

* GEM: Grup d'Emergències Mèdiques dels Bombers de la Generalitat.
* Based on [Billie Thompson's README template](https://gist.github.com/PurpleBooth/109311bb0361f32d87a2).
