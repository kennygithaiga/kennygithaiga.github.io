include <AccelStepper.h>

#define MotorInterfaceType 4
#define STEPS_PER_REV 2048 // Adjust based on your stepper motor

// Define stepper motor pins (IN1, IN3, IN2, IN4) using 2, 3, 4, 5
AccelStepper myStepper(MotorInterfaceType, 2, 4, 3, 5);

// Use HardwareSerial for SIM800L
#define sim800l Serial1  // Use Serial1 on Arduino Mega

void setup() {
    Serial.begin(9600);
    sim800l.begin(9600); // Initialize SIM800L on Serial1

    myStepper.setMaxSpeed(1000.0);
    myStepper.setAcceleration(50.0);
    myStepper.setSpeed(200);

    Serial.println("Initializing SIM800L...");
    delay(1000);

    sim800l.println("AT");
    updateSerial();
    sim800l.println("AT+CMGF=1"); // Set SMS text mode
    updateSerial();
    sim800l.println("AT+CNMI=1,2,0,0,0"); // New SMS indication
    updateSerial();
}

void loop() {
    if (sim800l.available()) {
        String sms = readSMS();
        Serial.print("Received SMS: ");
        Serial.println(sms);

        if (sms.toLowerCase().indexOf("open curtain") >= 0) {
            Serial.println("Opening Curtain...");
            myStepper.moveTo(STEPS_PER_REV); // Move forward
            while (myStepper.distanceToGo() != 0) {
                myStepper.run();
            }
            Serial.println("Curtain Open ✅");
        }
        else if (sms.toLowerCase().indexOf("close curtain") >= 0) {
            Serial.println("Closing Curtain...");
            myStepper.moveTo(-STEPS_PER_REV); // Move backward
            while (myStepper.distanceToGo() != 0) {
                myStepper.run();
            }
            Serial.println("Curtain Closed ✅");
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
