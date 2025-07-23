
#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

// Motor driver control pins
const int motorPin1 = 7; // IN1
const int motorPin2 = 8; // IN2

// To avoid multiple triggers within the same minute
bool hasRunMorning = false;
bool hasRunEvening = false;

void setup() {
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  stopMotor(); //This ensures motor is off at startup

  Wire.begin();
  rtc.begin();


}

void loop() {
  DateTime now = rtc.now();

  // Check for 8:00 AM trigger
  if (now.hour() == 8 && now.minute() == 0) {
    if (!hasRunMorning) {
      OpenBlind();
      hasRunMorning = true;
    }
  } else {
    hasRunMorning = false; // reset flag outside 8:00
  }

  // Check for closing trigger (22:00 in my case)
  if (now.hour() == 22 && now.minute() == 0) {
    if (!hasRunEvening) {
      CloseBlind();
      hasRunEvening = true;
    }
  } else {
    hasRunEvening = false; // reset flag after blinds close
  }

  delay(1000); // check every second
}

void OpenBlind() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  delay(6250);
  stopMotor();
}

void CloseBlind() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  delay(6250);
  stopMotor();
}

void stopMotor() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
}
