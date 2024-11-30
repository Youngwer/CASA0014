// 引脚定义
const int PIR_PIN = 3;      // PIR 信号输入
const int BUZZER_PIN = 7;   // 蜂鸣器输出

void setup() {
  pinMode(PIR_PIN, INPUT); // PIR 为输入模式
  pinMode(BUZZER_PIN, OUTPUT); // 蜂鸣器为输出模式
  Serial.begin(9600);
}

void loop() {
  int motionDetected = digitalRead(PIR_PIN); // 读取 PIR 信号

  if (motionDetected == HIGH) {
    // 产生声音（4kHz，持续 500 毫秒）
    tone(BUZZER_PIN, 4000, 500); 
    Serial.println("Motion detected! Buzzer ON.");
    delay(500); // 避免重复触发
  } else {
    noTone(BUZZER_PIN); // 停止蜂鸣器
    Serial.println("No motion.");
  }

  delay(500); // 延迟，避免过于频繁读取
}
