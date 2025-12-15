#include <Arduino.h>
#include "temperature.h"
#include "clock.h"
#include "MessageQueue.h"
#include "housekeep.h"



volatile int HouseKeeper::housekept;

void HouseKeeper::houseKeep(Message *msg) {
  housekept++;
  if(housekept%10 != 0)
    return;
  systemQueue.send(MessageType::TemperatureRequest,(char) 0);
  systemQueue.send(MessageType::Housekeep,"housekeeping");
}


HouseKeeper::HouseKeeper() {
  systemQueue.registerListener(MessageType::OneHz,&houseKeep);
}

HouseKeeper houseKeeper;
