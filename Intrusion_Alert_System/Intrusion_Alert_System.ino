#include <WiFiNINA.h>   
#include <PubSubClient.h>
#include <utility/wifi_drv.h>
#include "arduino_secrets.h" 

// WiFi and MQTT Configuration
const char* ssid          = SECRET_SSID;       // WiFi network name
const char* password      = SECRET_PASS;       // WiFi password
const char* mqtt_username = SECRET_MQTTUSER;   // MQTT username
const char* mqtt_password = SECRET_MQTTPASS;   // MQTT password
const char* mqtt_server = "mqtt.cetools.org";  // MQTT server address
const int mqtt_port = 1884;                    // MQTT port number
int status = WL_IDLE_STATUS;                   // WiFi connection status

// MQTT and WiFi clients
WiFiClient mkrClient;
PubSubClient client(mkrClient);

// MQTT topic for controlling the lights
char mqtt_topic_demo[] = "student/CASA0014/light/32/pixel/";
const char* brightness_topic = "student/CASA0014/light/32/brightness/";

// PIR and buzzer configuration
const int PIR_PIN = 3;      // PIR motion sensor pin
const int BUZZER_PIN = 7;   // Buzzer pin
const int BUTTON_PIN = 6;   // Reset button pin 
bool pirTriggered = false;  // Tracks if motion has been detected

void setup() {
  Serial.begin(115200);         // Initialize serial communication
  WiFi.setHostname("Lumina ucjtdjw");
  pinMode(PIR_PIN, INPUT);      // Set PIR sensor as input
  pinMode(BUZZER_PIN, OUTPUT);  // Set buzzer as output
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Set button as input with internal pull-up
  startWifi();                  // Connect to WiFi
  client.setServer(mqtt_server, mqtt_port); // Configure MQTT server
  Serial.println("Setup complete");

  // Set initial light state to green (safe)
  updateLights(0, 255, 0);
}

void loop() {
  if (!client.connected()) reconnectMQTT();    // Ensure MQTT connection
  if (WiFi.status() != WL_CONNECTED) startWifi(); // Ensure WiFi connection

  client.loop();   // Process incoming MQTT messages
  // Check if the reset button is pressed
  if (digitalRead(BUTTON_PIN) == LOW) {        // Button pressed
    resetSystem();                             // Reset lights to green
  }

  int motionDetected = digitalRead(PIR_PIN); // Check PIR sensor status

  if (motionDetected == HIGH) {
    // Trigger alarm
    tone(BUZZER_PIN, 1000, 250); // Emit sound with buzzer
    Serial.println("Motion detected! Lights flashing RED.");
    pirTriggered = true;         // Mark motion as detected
    flashRedLights();            // Flash red lights
  } else {
    noTone(BUZZER_PIN);          // Stop buzzer sound
    if (pirTriggered) {
      Serial.println("Motion was detected earlier. Lights turning YELLOW.");
      updateLights(204, 204, 0); // Change light to yellow (caution)
    } else {
      Serial.println("No motion detected. Lights turning GREEN.");
      updateLights(0, 255, 0);   // Change light to green (safe)
    }
    delay(200);                  // Avoid frequent updates
  }
}

// Flash red lights with buzzer sound
void flashRedLights() {
  for (int i = 0; i < 6; i++) { 
    updateLights(255, 0, 0); // Turn lights red
    tone(BUZZER_PIN, 1000);  // Emit sound
    delay(200);              // Flash delay

    updateLights(0, 0, 0);   // Turn lights off
    noTone(BUZZER_PIN);      // Stop sound
    delay(200);              // Pause before next flash
  }
}

// Reset system to green light 
void resetSystem() { 
  Serial.println("Reset button pressed. Lights turning GREEN."); // 
  pirTriggered = false;       // Clear motion detection flag 
  updateLights(0, 255, 0);    // Reset light to green 
}

// Update lights to specified RGB values
void updateLights(int R, int G, int B) {
  char mqtt_message[100];
  for (int pixelid = 0; pixelid < 12; pixelid++) {
    sprintf(mqtt_message, "{\"pixelid\": %d, \"R\": %d, \"G\": %d, \"B\": %d, \"W\": 0}", pixelid, R, G, B);
    client.publish(mqtt_topic_demo, mqtt_message); // Publish message to MQTT
  }
}

// Connect to WiFi network
void startWifi() {
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("WiFi module not found!");
    while (true);
  }
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println("WiFi connected");
}

// Reconnect to MQTT broker if connection is lost
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    String clientId = "LuminaSelector-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT!");
    } else {
      Serial.print("Failed to connect. Retrying...");
      delay(5000);
    }
  }
}
