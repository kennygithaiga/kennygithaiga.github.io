#include <SoftwareSerial.h>

SoftwareSerial gsm(10, 11);  // RX, TX

void setup() {
    Serial.begin(9600);  // Serial monitor
    gsm.begin(9600);     // GSM module baud rate
    Serial.println("Initializing GSM module...");
    delay(1000);
    
    gsm.println("AT");  // Test GSM response
    delay(1000);
    
    if (gsm.available()) {
        Serial.println("GSM Module is responding!");
    } else {
        Serial.println("No response. Check wiring and power.");
    }
}

void loop() {
    while (gsm.available()) {
        Serial.write(gsm.read());  // Print GSM response
    }
}
