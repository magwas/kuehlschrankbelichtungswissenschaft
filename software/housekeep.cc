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
  if(housekept%10 != 0)
    return;
  clock.printTime();
  systemQueue.send(MessageType::TemperatureRequest,(char) 0);
  systemQueue.send(MessageType::Console,"housekeeping");
}


HouseKeeper::HouseKeeper() {
  pinMode(INT_1S, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INT_1S), interrupt_1S, FALLING);
  systemQueue.registerListener(MessageType::OneHz,&houseKeep);
}

HouseKeeper houseKeeper;
