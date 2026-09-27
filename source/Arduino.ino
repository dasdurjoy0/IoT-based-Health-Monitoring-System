#include <Wire.h>
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "MAX30105.h"

// ---------------- LCD ----------------
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// ---------------- DS18B20 (temperature) ----------------
#define ONE_WIRE_BUS 9
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensor(&oneWire);

// ---------------- MAX30102 (SpO2/HR) ----------------
MAX30105 particleSensor;

// ---------------- State ----------------
float bodyTemp = 0.0;
int spo2 = 0;

unsigned long lastDisplay = 0;
unsigned long lastTempRead = 0;
unsigned long lastSend = 0;

const int fingerThreshold = 15000;

void setup() {
  Serial.begin(9600);   // Data line to ESP

  lcd.begin(16, 2);
  lcd.print("Initializing...");
  delay(1200);

  tempSensor.begin();

  Wire.begin();
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    lcd.clear();
    lcd.print("MAX30102 ERROR");
    while (1);
  }

  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x3F);
  particleSensor.setPulseAmplitudeIR(0x3F);
  particleSensor.setPulseAmplitudeGreen(0);

  lcd.clear();
  lcd.print("Place finger...");
}

void loop() {
  unsigned long now = millis();

  // ----- Temperature (every 2s) -----
  if (now - lastTempRead > 2000) {
    lastTempRead = now;
    tempSensor.requestTemperatures();
    float t = tempSensor.getTempCByIndex(0);
    if (t > -20 && t < 60) {
      bodyTemp = t;
    }
  }

  // ----- SpO2 -----
  long ir = particleSensor.getIR();
  long red = particleSensor.getRed();

  if (ir < fingerThreshold) {
    // No finger detected
    spo2 = 0;
    if (now - lastDisplay > 300) {
      lastDisplay = now;
      lcd.setCursor(0, 0);
      lcd.print("Place finger    ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
    }
    return; // skip the rest of loop() until a finger is placed
  }

  // NOTE: this ratio formula is a rough placeholder, NOT a calibrated
  // SpO2 measurement. It gives a plausible-looking number for demo
  // purposes only. Do not treat this as medically accurate.
  float ratio = (float)red / (float)ir;
  int calcSpO2 = 110 - (25 * ratio);
  if (calcSpO2 > 100) calcSpO2 = 100;
  if (calcSpO2 < 80)  calcSpO2 = 80;
  spo2 = calcSpO2;

  // ----- LCD update (every 300ms) -----
  if (now - lastDisplay > 300) {
    lastDisplay = now;

    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(bodyTemp, 1);
    lcd.print((char)223); // degree symbol
    lcd.print("C    ");

    lcd.setCursor(0, 1);
    lcd.print("SpO2:");
    lcd.print(spo2);
    lcd.print("%     ");
  }

  // ----- Send to ESP (every 5s) -----
  // Protocol: *TEMP:<float>,SPO2:<int>#
  if (now - lastSend > 5000) {
    lastSend = now;

    Serial.print("*TEMP:");
    Serial.print(bodyTemp, 1);
    Serial.print(",SPO2:");
    Serial.print(spo2);
    Serial.println("#");
  }
}
