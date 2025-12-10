#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "userinterface.h"
#include "temperature.h"

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


Thermometer::Thermometer() {
     sensors.begin();
     systemQueue.registerListener(MessageType::TemperatureRequest,&read);
}

void Thermometer::read(Message * msg) {
    sensors.requestTemperatures();
    float tempC = sensors.getTempCByIndex(msg->payload[0]);
    systemQueue.send(MessageType::Temperature,tempC);
}

