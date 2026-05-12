#define MQ2_D0 2   // Digital output pin from MQ-2
#define BUZZER 8   // Buzzer pin

void setup() {
    pinMode(MQ2_D0, INPUT);
    pinMode(BUZZER, OUTPUT);
    Serial.begin(9600);
}

void loop() {
    int gasDetected = digitalRead(MQ2_D0); // Read D0 (HIGH or LOW)
    
    if (gasDetected == HIGH) { // Gas detected
        Serial.println("⚠️ Gas Detected! Buzzer ON!");
        digitalWrite(BUZZER, HIGH); // Turn buzzer ON
    } else {
        Serial.println("✅ No Gas Detected.");
        digitalWrite(BUZZER, LOW); // Turn buzzer OFF
    }

    delay(500); // Small delay to avoid rapid switching
}
