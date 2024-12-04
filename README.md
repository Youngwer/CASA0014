# Intrusion Alarm System
## 0. Project Overview
This Intrusion Alarm System is an Arduino-based project designed to detect unauthorized movements in environments such as labs, offices, or homes. The status of the system will be sent to MQTT server via the Wi-Fi network "CE-Hub-Student", ensuring both on-site and off-site monitoring of activity. This project about MQTT server builds upon [CASA0014 Chrono Lumina workshop](https://workshops.cetools.org/codelabs/CASA0014-L6-chrono-blinker/index.html?index=..%2F..casa0014#1), with sensor added to the system to monitor its surroundings.

## Prototype and System Design 
### Workflow
The following flowchart illustrates the working process of the Intrusion Alarm System:
![Circuit Diagram](image/flow%20chart.png)
Before deploying the Intrusion Alarm System, ensure necessary setup and libraries installed.

### Prerequisites  
**Hardware:**  
   + Arduino MKR WiFi 1010  
   + PIR Motion Sensor  
   + PKM22EPP-40 Buzzer  
   + Button (for resetting system)  
   + 10kΩ Resistor (for the button's pull-up configuration)  
   + Dupont Wires (for connections) 

**Software:**  
   + Arduino IDE
   + Libraries
     - `WiFiNINA`  
     - `PubSubClient`
     - `wifi_drv` 
     - `arduino_secrets.h`

### Process
#### [MQTT Connection Test](MQTT_ChronoLumina_test/MQTT_ChronoLumina_test.ino)
Before integrating the full system, the MQTT connection was tested to ensure that the Arduino could connect to the MQTT broker. This test is based on the [CASA0014 Chrono Lumina workshop](https://workshops.cetools.org/codelabs/CASA0014-L6-chrono-blinker/index.html?index=..%2F..casa0014#1), which outlines how to set up MQTT communication in an Arduino environment.

In this test, the goal was to verify that the Arduino MKR WiFi 1010 can connect to both Wi-Fi and an MQTT server, ensuring reliable communication for subsequent integration into the Intrusion Alarm System.
#### [Buzzer and PIR Sensor Test](Alarm_PIR_test/Alarm_PIR_test.ino)
The PIR sensor was used to detect motion in the system's environment, and the buzzer was activated in response to motion detection. The goal was to confirm that the hardware components were functioning correctly before integrating them into the full system.

#### [Intrusion Alert System](Intrusion_Alert_System/Intrusion_Alert_System.ino)
The entire Intrusion Alarm System was assembled, including:

+ Connecting the Arduino to Wi-Fi and the MQTT server.
+ Continuously monitoring the environment for motion using the PIR sensor.
+ Triggering the buzzer and visual indicators (lights) based on motion detection.
### circuit diagram
Below is the circuit diagram for the Intrusion Alarm System:
![Circuit Diagram](image/circuit%20diagram.png)