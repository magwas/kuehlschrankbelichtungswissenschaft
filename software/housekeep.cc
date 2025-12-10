#include <Arduino.h>
#include "temperature.h"
#include "clock.h"
#include "MessageQueue.h"
#include "housekeep.h"


void HouseKeeper::interrupt_1S() {
    systemQueue.send(MessageType::OneHz,(char)0);
}

volatile int HouseKeeper::housekept;

void HouseKeeper::houseKeep(Message *msg) {
  housekept++;
  if(housekept%4 != 0)
    return;
  clock.printTime();
  systemQueue.send(MessageType::TemperatureRequest,(char) 0);
  Serial.println(" houseekeping");
}


HouseKeeper::HouseKeeper() {
  pinMode(INT_1S, INPUT_PULLUP);
  Serial.println(digitalPinToInterrupt(INT_1S));
  attachInterrupt(digitalPinToInterrupt(INT_1S), interrupt_1S, FALLING);
  systemQueue.registerListener(MessageType::OneHz,&houseKeep);
}

HouseKeeper houseKeeper;
