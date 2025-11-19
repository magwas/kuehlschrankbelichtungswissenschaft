#include <Arduino.h>
#include "userinterface.h"
#include "lamp.h"
void userInterface()
{
  String input = Serial.readStringUntil('\n');
  if(input.equals("on")) {
    lamp_turnOn();
    Serial.print(" turned on ");
  }
  if(input.equals("off")) {
    lamp_turnOff();
    Serial.print(" turned off ");
  }
  if(input.startsWith("set")) {
    int spc1 = input.indexOf(" ");
    if(-1 != spc1) {
      int spc2 = input.indexOf(" ",spc1+1);
      if(-1 != spc2) {
        Serial.print(spc1);
        Serial.print(" ");
        Serial.print(spc2);
        Serial.print(" ");
        String arg1 = input.substring(spc1,spc2);
        String arg2 = input.substring(spc2);
        int ppf65 = arg1.toInt();
        int ppf18 = arg2.toInt();
        Serial.print(ppf65);
        Serial.print(" ");
        Serial.print(ppf18);
        Serial.print(" ");
        lamp_setPPF(ppf65,ppf18);
      } else {
        Serial.print(input);
        Serial.print(" ");
        Serial.print(spc1);
        Serial.print(" ");
        Serial.print("no 2nd space in ");
      }
    } else {
      Serial.print("no space in ");
    }
  }
}
