const int PIR_PIN = 3;      // PIR motion sensor pin
const int BUZZER_PIN = 7;   // Buzzer pin

void setup() {
  pinMode(PIR_PIN, INPUT);      // Configure PIR as input
  pinMode(BUZZER_PIN, OUTPUT);  // Configure buzzer as output
  Serial.begin(9600);           // Initialize serial communication
}

void loop() {
  int motionDetected = digitalRead(PIR_PIN); // Read PIR sensor

  if (motionDetected == HIGH) {
    tone(BUZZER_PIN, 4000, 500); // Emit sound (4kHz for 500ms)
    Serial.println("Motion detected! Buzzer ON.");
    delay(500);                  // Avoid frequent triggers
  } else {
    noTone(BUZZER_PIN);          // Stop buzzer
    Serial.println("No motion.");
  }

  delay(500); // Pause between sensor reads
}
