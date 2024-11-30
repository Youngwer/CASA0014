#include <WiFiNINA.h>   
#include <PubSubClient.h>
#include <utility/wifi_drv.h>
#include "arduino_secrets.h" 

const char* ssid          = SECRET_SSID;
const char* password      = SECRET_PASS;
const char* mqtt_username = SECRET_MQTTUSER;
const char* mqtt_password = SECRET_MQTTPASS;
const char* mqtt_server = "mqtt.cetools.org";
const int mqtt_port = 1884;
int status = WL_IDLE_STATUS;

WiFiClient mkrClient;
PubSubClient client(mkrClient);

// MQTT topic for controlling the light
char mqtt_topic_demo[] = "student/CASA0014/light/32/pixel/";
const char* brightness_topic = "student/CASA0014/light/32/brightness/";

// PIR and buzzer setup
const int PIR_PIN = 3;      // PIR motion sensor pin
const int BUZZER_PIN = 7;   // Buzzer pin

bool pirTriggered = false;  // 记录 PIR 是否被触发过

void setup() {
  // Initialize serial, WiFi, and MQTT
  Serial.begin(115200);
  WiFi.setHostname("Lumina ucjtdjw");
  pinMode(PIR_PIN, INPUT); 
  pinMode(BUZZER_PIN, OUTPUT);
  
  startWifi();
  client.setServer(mqtt_server, mqtt_port);
  Serial.println("Setup complete");

  // 初始状态设置为绿色
  updateLights(0, 255, 0); // 安全状态，绿色
}

void loop() {
  // Ensure WiFi and MQTT connection
  if (!client.connected()) reconnectMQTT();
  if (WiFi.status() != WL_CONNECTED) startWifi();
  
  client.loop();

  // Check PIR sensor and update lights
  int motionDetected = digitalRead(PIR_PIN);

  if (motionDetected == HIGH) {
    // 触发报警
    tone(BUZZER_PIN, 1000, 250); // 蜂鸣器发声
    Serial.println("Motion detected! Lights flashing RED.");
    pirTriggered = true;    // 记录 PIR 已被触发
    flashRedLights();       // 闪烁红灯
  } else {
    // 停止蜂鸣器，更新灯状态
    noTone(BUZZER_PIN);

    if (pirTriggered) {
      Serial.println("Someone has been here. Lights turning YELLOW.");
      updateLights(204, 204, 0); // PIR 曾被触发状态，黄色
    } else {
      Serial.println("Nothing detected. Lights turning GREEN.");
      updateLights(0, 255, 0); // 安全状态，绿色
    }

    delay(200);
  }
}

void flashRedLights() {
  for (int i = 0; i < 6; i++) { // 红灯和蜂鸣器快速闪烁 3 次
    // 红灯亮
    updateLights(255, 0, 0); 
    tone(BUZZER_PIN, 1000); // 蜂鸣器发声
    delay(200);             // 快速延时，蜂鸣器保持 100 毫秒

    // 红灯灭
    updateLights(0, 0, 0);  
    noTone(BUZZER_PIN);     // 停止蜂鸣器
    delay(200);             // 快速延时，蜂鸣器静音 100 毫秒
  }
}


void updateLights(int R, int G, int B) {
  char mqtt_message[100];

  for (int pixelid = 0; pixelid < 12; pixelid++) {
    // 构建每个 pixelid 的颜色消息
    sprintf(mqtt_message, "{\"pixelid\": %d, \"R\": %d, \"G\": %d, \"B\": %d, \"W\": 0}", pixelid, R, G, B);
    if (client.publish(mqtt_topic_demo, mqtt_message)) {
    } else {
    }
  }
}


void startWifi() {
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("WiFi module failed!");
    while (true);
  }
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    String clientId = "LuminaSelector";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT!");
    } else {
      Serial.print("Failed, rc=");
      Serial.println(client.state());
      delay(5000);
    }
  }
}
