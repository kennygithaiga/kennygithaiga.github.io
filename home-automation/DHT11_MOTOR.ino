#include <DHT.h>

// Pin Definitions
#define DHT_PIN 10      // DHT11 Sensor data pin
#define RELAY_PIN 9     // Relay controlling the motor
#define TEMP_THRESHOLD 20  // Temperature threshold (°C)
#define HUM_THRESHOLD 90// Humidity threshold (%)

// DHT Sensor Setup
DHT dht(DHT_PIN, DHT11);

void setup() {
    Serial.begin(9600);
    Serial.println("DHT11 & Motor Control Starting...");

    pinMode(RELAY_PIN, OUTPUT);  
    dht.begin();  // Initialize DHT sensor
}

void loop() {
    // Read Temperature & Humidity
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    // Check if readings are valid
    if (isnan(temp) || isnan(hum)) {
        Serial.println("❌` Failed to read from sensor! Check wiring.");
        return;  // Exit loop iteration and retry
    }

    // Print Sensor Data
    Serial.print("🌡️ Temperature: ");
    Serial.print(temp);
    Serial.println(" °C");

    Serial.print("💧 Humidity: ");
    Serial.print(hum);
    Serial.println(" %");

    // Determine Motor ON/OFF
    if (temp > TEMP_THRESHOLD || hum > HUM_THRESHOLD) {
        digitalWrite(RELAY_PIN, HIGH);  // Motor ON
        Serial.println("⚡ Motor ON!");
    } else {
        digitalWrite(RELAY_PIN, LOW);   // Motor OFF
        Serial.println("❄️ Motor OFF!");
    }

    delay(2000);  // Wait 2 seconds before the next reading
}
