#include <Arduino.h>
#include "temperature.h"
#include "housekeep.h"
#include "userinterface.h"

void setup()
{
  Serial.begin(9600);
  temperatureInit();
  houseKeepInit();
}

void loop()
{
    if(Serial)
      userInterface();
    houseKeep();
    delay(100);
}


