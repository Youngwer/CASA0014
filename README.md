# CASA0014
Intrusion Alarm System
## 0. Introduction
This Intrusion Alarm System is an Arduino-based project designed to detect unauthorized movements in environments such as labs, offices, or homes.  
The status of the system will be sent to MQTT server via the Wi-Fi network "CE-Lab-Students", ensuring both on-site and off-site monitoring of activity.
This project about MQTT server builds upon [CASA0014 Chrono Lumina workshop](https://workshops.cetools.org/codelabs/CASA0014-L6-chrono-blinker/index.html?index=..%2F..casa0014#1), with sensor added to the system to monitor its surroundings.
## 1. Preparation
Before deploying the Intrusion Alarm System, ensure that you have the necessary setup and libraries installed.

### Prerequisites  
1. **Hardware:**  
   + Arduino MKR WiFi 1010  
   + PIR Motion Sensor  
   + PKM22EPP-40 Buzzer  
2. **Software:**  
   + Arduino IDE
   + Libraries
     - `WiFiNINA`  
     - `PubSubClient`
     - `wifi_drv` 
     - `arduino_secrets.h`

## 2. Process
### circuit diagram
Below is the circuit diagram for the Intrusion Alarm System:
![Circuit Diagram](image/circuit%20diagram.png)
