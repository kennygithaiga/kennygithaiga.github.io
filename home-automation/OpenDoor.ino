#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Keypad.h>

#define RST_PIN 49
#define SS_PIN 53

int servoPin = 13;
Servo servo;
int close = 0;
int open = 180;

int ledYell = 12;
int ledRed = 11;

byte readCard[4];
String MasterTag1 = "33FE28DA";   // First allowed Tag ID
String MasterTag2 = "D3FFF3F6"; // REPLACE with your second Tag ID!!!
String tagID = "";

const byte ROWS = 4; // Define the number of rows on the keypad
const byte COLS = 4; // Define the number of columns on the keypad
char keys[ROWS][COLS] = { // Matrix defining character to return for each key
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {10, 9, 8, 7}; //connect to the row pins (R0-R3) of the keypad
byte colPins[COLS] = {6, 5, 4, 3}; //connect to the column pins (C0-C2) of the keypad
//initialize an instance of class
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String enteredCode = "";
bool accessGranted = false; // Flag to indicate if access is granted

// Create instances
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 20, 4);

void grantAccess() {
    lcd.clear();
    lcd.print(" ACCESS GRANTED! ");
    digitalWrite(ledYell, HIGH);
    servo.write(open);
    delay(2500);
    digitalWrite(ledYell, LOW);
    delay(3000);
    servo.write(close);
    accessGranted = false; // Reset the flag
    lcd.clear();
    lcd.print(" Hello there... ");
    lcd.setCursor(0, 1);
    lcd.print("Pls scan your card");
    lcd.setCursor(0, 2);
    lcd.print("or use keypad>>");
}

void denyAccess() {
    lcd.clear();
    lcd.print(" ACCESS DENIED! ");
    lcd.setCursor(0, 1);
    lcd.print("Try again...");
    digitalWrite(ledRed, HIGH);
    delay(2000);
    digitalWrite(ledRed, LOW);
    lcd.clear();
    lcd.print(" Hello there... ");
    lcd.setCursor(0, 1);
    lcd.print("Pls scan your card");
    lcd.setCursor(0, 2);
    lcd.print("or use keypad>>");
}

void setup() {
    // Initiating
    servo.attach(servoPin);
    servo.write(close);

    lcd.init();
    lcd.backlight();

    pinMode(ledYell, OUTPUT);
    pinMode(ledRed, OUTPUT);

    SPI.begin();         // SPI bus
    mfrc522.PCD_Init();   // MFRC522
    lcd.begin(20, 4);     // LCD screen

    lcd.clear();
    lcd.print(" Hello there... ");
    lcd.setCursor(0, 1);
    lcd.print("Pls scan your card");
    lcd.setCursor(0, 2);
    lcd.print("or use keypad>>");
}

void loop() {
    char key = keypad.getKey(); // Read the key

    // Check for RFID card
    if (getID()) {
        lcd.clear();
        lcd.setCursor(0, 0);

        if (tagID == MasterTag1 || tagID == MasterTag2) { // Check against allowed tags
            accessGranted = true;
        } else {
            denyAccess();
        }
    }

    // Process keypad input
    if (key) { // If a key is pressed
        Serial.print("Key Pressed : ");
        Serial.println(key);

        enteredCode += key; // Append the pressed key to the entered code
        lcd.setCursor(0, 3);
        lcd.print("Code: ");
        lcd.print(enteredCode);

        if (enteredCode.length() >= 4) { //check if 4 digits have been entered.
            if (enteredCode == "1234") {
                accessGranted = true;
            } else {
                denyAccess();
            }
            enteredCode = ""; // Reset entered code for next input
        }
    }

    // Grant access if either RFID or keypad is successful
    if (accessGranted) {
        grantAccess();
    }

    delay(50); // Small delay for debouncing and to prevent excessive reads
}

//Read new tag if available
boolean getID() {
    // Getting ready for Reading PICCs
    if (!mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
        return false;
    }
    if (!mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
        return false;
    }
    tagID = "";
    for (uint8_t i = 0; i < 4; i++) { // The MIFARE PICCs that we use have 4 byte UID
        //readCard[i] = mfrc522.uid.uidByte[i];
        tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
    }
    tagID.toUpperCase();
    mfrc522.PICC_HaltA(); // Stop reading
    return true;
}#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Keypad.h>

#define RST_PIN 49
#define SS_PIN 53

int servoPin = 13;
Servo servo;
int close = 0;
int open = 180;

int ledYell = 12;
int ledRed = 11;

byte readCard[4];
String MasterTag1 = "33FE28DA";   // First allowed Tag ID
String MasterTag2 = "D3FFF3F6"; // REPLACE with your second Tag ID!!!
String tagID = "";

const byte ROWS = 4; // Define the number of rows on the keypad
const byte COLS = 4; // Define the number of columns on the keypad
char keys[ROWS][COLS] = { // Matrix defining character to return for each key
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {10, 9, 8, 7}; //connect to the row pins (R0-R3) of the keypad
byte colPins[COLS] = {6, 5, 4, 3}; //connect to the column pins (C0-C2) of the keypad
//initialize an instance of class
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String enteredCode = "";
bool accessGranted = false; // Flag to indicate if access is granted

// Create instances
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 20, 4);

void grantAccess() {
    lcd.clear();
    lcd.print(" ACCESS GRANTED! ");
    digitalWrite(ledYell, HIGH);
    servo.write(open);
    delay(2500);
    digitalWrite(ledYell, LOW);
    delay(3000);
    servo.write(close);
    accessGranted = false; // Reset the flag
    lcd.clear();
    lcd.print(" Hello there... ");
    lcd.setCursor(0, 1);
    lcd.print("Pls scan your card");
    lcd.setCursor(0, 2);
    lcd.print("or use keypad>>");
}

void denyAccess() {
    lcd.clear();
    lcd.print(" ACCESS DENIED! ");
    lcd.setCursor(0, 1);
    lcd.print("Try again...");
    digitalWrite(ledRed, HIGH);
    delay(2000);
    digitalWrite(ledRed, LOW);
    lcd.clear();
    lcd.print(" Hello there... ");
    lcd.setCursor(0, 1);
    lcd.print("Pls scan your card");
    lcd.setCursor(0, 2);
    lcd.print("or use keypad>>");
}

void setup() {
    // Initiating
    servo.attach(servoPin);
    servo.write(close);

    lcd.init();
    lcd.backlight();

    pinMode(ledYell, OUTPUT);
    pinMode(ledRed, OUTPUT);

    SPI.begin();         // SPI bus
    mfrc522.PCD_Init();   // MFRC522
    lcd.begin(20, 4);     // LCD screen

    lcd.clear();
    lcd.print(" Hello there... ");
    lcd.setCursor(0, 1);
    lcd.print("Pls scan your card");
    lcd.setCursor(0, 2);
    lcd.print("or use keypad>>");
}

void loop() {
    char key = keypad.getKey(); // Read the key

    // Check for RFID card
    if (getID()) {
        lcd.clear();
        lcd.setCursor(0, 0);

        if (tagID == MasterTag1 || tagID == MasterTag2) { // Check against allowed tags
            accessGranted = true;
        } else {
            denyAccess();
        }
    }

    // Process keypad input
    if (key) { // If a key is pressed
        Serial.print("Key Pressed : ");
        Serial.println(key);

        enteredCode += key; // Append the pressed key to the entered code
        lcd.setCursor(0, 3);
        lcd.print("Code: ");
        lcd.print(enteredCode);

        if (enteredCode.length() >= 4) { //check if 4 digits have been entered.
            if (enteredCode == "1234") {
                accessGranted = true;
            } else {
                denyAccess();
            }
            enteredCode = ""; // Reset entered code for next input
        }
    }

    // Grant access if either RFID or keypad is successful
    if (accessGranted) {
        grantAccess();
    }

    delay(50); // Small delay for debouncing and to prevent excessive reads
}

//Read new tag if available
boolean getID() {
    // Getting ready for Reading PICCs
    if (!mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
        return false;
    }
    if (!mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
        return false;
    }
    tagID = "";
    for (uint8_t i = 0; i < 4; i++) { // The MIFARE PICCs that we use have 4 byte UID
        //readCard[i] = mfrc522.uid.uidByte[i];
        tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
    }
    tagID.toUpperCase();
    mfrc522.PICC_HaltA(); // Stop reading
    return true;
}