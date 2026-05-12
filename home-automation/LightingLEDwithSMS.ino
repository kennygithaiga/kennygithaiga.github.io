#include <SoftwareSerial.h>

// Create Software Serial for SIM800L on pins 10 (Tx) & 11 (Rx)
SoftwareSerial sim800l(10, 11); // (RX,TX) SIM800L Tx to Arduino 10, Rx to Arduino 11
int relay1=7;
int relay2=6;

void setup() {
    Serial.begin(9600);
    sim800l.begin(9600);

    Serial.println("Initializing SIM800L...");
    delay(1000);

    sim800l.println("AT");
    updateSerial();
    sim800l.println("AT+CMGF=1"); // Set SMS text mode
    updateSerial();
    sim800l.println("AT+CNMI=1,2,0,0,0"); // New SMS indication
    updateSerial();
    pinMode(relay1,OUTPUT);
    digitalWrite(relay1,HIGH);
    pinMode(relay2,OUTPUT);
    digitalWrite(relay2,LOW);
}

void loop() {
  if (sim800l.available()) {
    String sms = readSMS();
    Serial.print("Received SMS: ");
    Serial.println(sms);

    String lowerSms = sms;
    lowerSms.toLowerCase();

    if (lowerSms.indexOf("led1") >= 0) {
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, LOW);
      Serial.println("Led1 On!!");
    } else if (lowerSms.indexOf("led2") >= 0) {
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, HIGH);
      Serial.println("LED2 ON!!!");
    } else if (lowerSms.indexOf("both on") >= 0) {
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, HIGH);
      Serial.println("BOTH ON!!!");
    } else if (lowerSms.indexOf("both off") >= 0) {
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, LOW);
      Serial.println("BOTH OFF!!!!");
    } else {
      Serial.println("Invalid Input");
    }
  }
}

// Function to read SMS from SIM800L
String readSMS() {
    String message = "";
    while (sim800l.available()) {
        char c = sim800l.read();
        message += c;
        delay(10);
    }
    return message;
}

// Function to forward Serial data
void updateSerial() {
    delay(500);
    while (Serial.available()) {
        sim800l.write(Serial.read());
    }
    while (sim800l.available()) {
        Serial.write(sim800l.read());
    }
}
