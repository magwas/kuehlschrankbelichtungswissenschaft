#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "userinterface.h"
#include "temperature.h"

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float tempC;

void temperatureInit() {
      sensors.begin();
}

void temperatureGet() {
  sensors.requestTemperatures();
   tempC = sensors.getTempCByIndex(0);
}

void temperaturePrint() {
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println("°C");
}