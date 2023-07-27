#include <Wire.h>
#include <LiquidCrystal_I2C.h>

float currentPressureReading;
float lastPressureReading;
bool debugmode = 0;
float MapBarPressure;
#define relayOFF LOW
const int alarm = 2;
volatile byte relayState = LOW;
int wid = 16;
char result[8];
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Pressure (Bar)");
  pinMode(alarm, OUTPUT);
}

void loop() {
  updatePressure();
  currentPressureReading = analogRead(A0);
  float voltage = currentPressureReading * (5.0 / 1024.0);
  MapBarPressure = map(currentPressureReading * 10, 0, 1023, 0, 8);

  if (debugmode) {
    Serial.println("***** DEBUG INFORMATION *****");
    Serial.print("Raw Sensor Value = ");
    Serial.println(currentPressureReading);
    Serial.print("Pressure in Bar = ");
    Serial.println(MapBarPressure / 10);
    Serial.print("Input voltage = ");
    Serial.println(voltage);
    Serial.print('\n');
    Serial.print('\n');
    delay(4000);
  }
}

void updatePressure() {
  
  delay(4000);
  float converted = MapBarPressure / 10;
  dtostrf(converted, 6, 2, result);
  int len = strlen(result);
  lcd.setCursor((16 - len) / 2, 1);
  lcd.print(result);

  if ((converted > 0.00) && (converted < 6.00)) {
    digitalWrite(alarm, LOW);
    if (debugmode) {
      Serial.println("***** Too Low *******");
      Serial.print("Pressure in Bar = ");
      Serial.println(converted);
      Serial.print('\n');
      Serial.print('\n');
    }    
  }

  else {
    digitalWrite(alarm, HIGH);
    if (debugmode) {
    Serial.println("***** Pressure OK *****");
    Serial.print("Pressure in Bar = ");
    Serial.println(converted);
    Serial.print('\n');
    Serial.print('\n');
    }
   
    
  }
}