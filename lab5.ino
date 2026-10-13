
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include <math.h>
#include "DHT.h"

LiquidCrystal_PCF8574 lcd(0x27);
int show = -1;

byte dotOff[] = {
  0b00000, 0b01110, 0b10001, 0b10001,
  0b10001, 0b01110, 0b00000, 0b00000
};

#define DHTPIN 12
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

float calculateHeatIndex(int tempC, int h);

void setup() {
  Serial.begin(9600);

  Wire.begin();
  Wire.beginTransmission(0x27);
  int error = Wire.endTransmission();

  if (error == 0) {
    show = 0;
    lcd.begin(16, 2);
    lcd.createChar(1, dotOff);
    lcd.setBacklight(255);
  }

  dht.begin();
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    delay(1000);
    return;
  }

  int h = constrain((int)(round(humidity / 10.0) * 10), 0, 100);
  int t = (int)temperature;

  float hi = calculateHeatIndex(t, h);

  Serial.print(F("T "));
  Serial.print(t);
  Serial.print(F("*C  RH "));
  Serial.print(h);
  Serial.print(F("%  HI "));
  Serial.println(hi);

  if (show == 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T ");
    lcd.print(t);
    lcd.print("*C RH ");
    lcd.print(h);
    lcd.print("%");

    lcd.setCursor(0, 1);
    lcd.print("H ");
    lcd.print(hi);

    lcd.print(" ");

    if (hi >= 52) {
      lcd.print("Extreme");
    } else if (hi >= 42) {
      lcd.print("Danger");
    } else if (hi >= 33) {
      lcd.print("Ex Caution");
    } else if (hi >= 27) {
      lcd.print("Caution");
    }
  }

  delay(1000);
}

float calculateHeatIndex(int tempC, int h) {
  if (h == 0) {
    return tempC - 4;
  } else if (h == 10) {
    return tempC - 4;
  } else if (h == 20) {
    return tempC - 3;
  } else if (h == 30) {
    return tempC - 3;
  } else if (h == 40) {
    return tempC - 2;
  } else if (h == 50) {
    return tempC - 1;
  } else if (h == 60) {
    return tempC;
  } else if (h == 70) {
    return tempC + 1;
  } else if (h == 80) {
    return tempC + 2;
  } else if (h == 90) {
    return tempC + 2;
  } else if (h == 100) {
    return tempC - 3;
  } else {
    return 0;
  }
}
