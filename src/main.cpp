#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;

bool doorOpen = true;

void setup() {
  lcd.init();
  lcd.backlight();

  myServo.setPeriodHertz(50);
  myServo.attach(18);

  myServo.write(0); // initial (door open)
}

void loop() {

  if (doorOpen) {

    // Countdown with OPEN status
    for (int i = 3; i > 0; i--) {
      lcd.clear();
      
      lcd.setCursor(0, 0);
      lcd.print("Door: OPEN");
      
      lcd.setCursor(0, 1);
      lcd.print("Time: ");
      lcd.print(i);
      lcd.print("s");

      delay(1000);
    }

    // Warning
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Warning!!!");
    delay(1500);

    // Auto closing message
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Auto Closing...");

    // Smooth closing (0 → 90)
    for (int pos = 0; pos <= 90; pos++) {
      myServo.write(pos);
      delay(15);
    }

    delay(1000);

    // Smooth return (90 → 0)
    for (int pos = 90; pos >= 0; pos--) {
      myServo.write(pos);
      delay(15);
    }

    doorOpen = false;
  }

  else {
    // CLOSED status
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Door: CLOSED");
    
    lcd.setCursor(0, 1);
    lcd.print("Safe :)");

    delay(3000);
  }
}