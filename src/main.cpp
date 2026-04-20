#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// ------------------- PIN CONFIG -------------------
#define DHTPIN 4          // Change if needed
#define DHTTYPE DHT11
#define RELAY_PIN 5       // Relay pin

// ------------------- OBJECTS -------------------
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ------------------- SETTINGS -------------------
float tempThreshold = 30.0;   // Adjust based on testing

void setup() {
    Serial.begin(115200);

    // Initialize components
    dht.begin();
    lcd.init();
    lcd.backlight();

    pinMode(RELAY_PIN, OUTPUT);

    // IMPORTANT: Default OFF (depends on relay type)
    digitalWrite(RELAY_PIN, LOW);

    lcd.setCursor(0, 0);
    lcd.print("Smart Fridge");
    lcd.setCursor(0, 1);
    lcd.print("Initializing...");
    delay(2000);
    lcd.clear();
}

void loop() {
    // -------- READ SENSOR --------
    float temp = dht.readTemperature();
    float hum  = dht.readHumidity();

    // -------- ERROR CHECK --------
    if (isnan(temp) || isnan(hum)) {
        Serial.println("DHT Error!");

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Sensor Error!");
        delay(2000);
        return;
    }

    // -------- SERIAL DEBUG --------
    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.print(" °C | Hum: ");
    Serial.print(hum);
    Serial.println(" %");

    // -------- DISPLAY --------
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(temp);
    lcd.print("C ");

    lcd.setCursor(0, 1);
    lcd.print("H:");
    lcd.print(hum);
    lcd.print("% ");

    // -------- CONTROL LOGIC --------
    if (temp > tempThreshold) {
        // Cooling ON
        digitalWrite(RELAY_PIN, HIGH);

        lcd.setCursor(10, 1);
        lcd.print("ON ");

        Serial.println("Cooling ON");
    } else {
        // Cooling OFF
        digitalWrite(RELAY_PIN, LOW);

        lcd.setCursor(10, 1);
        lcd.print("OFF");

        Serial.println("Cooling OFF");
    }

    delay(2000);
}