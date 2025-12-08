#include <Arduino.h>
#include "temperature.h"
#include "clock.h"
volatile byte shouldHouseKeep = 0;

void interrupt_1S() {
  shouldHouseKeep = 1;
}

void houseKeepInit() {
  pinMode(INT_1S, INPUT_PULLUP);
  Serial.println(digitalPinToInterrupt(INT_1S));
  attachInterrupt(digitalPinToInterrupt(INT_1S), interrupt_1S, FALLING);
}

void houseKeep() {
  if(!shouldHouseKeep)
    return;
  temperatureGet();
  temperaturePrint();
  Serial.println (millis());
  printTime();
  Serial.println(" houseekeping");
  shouldHouseKeep = 0;
}
