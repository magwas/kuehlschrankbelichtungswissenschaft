#include <Arduino.h>
#include "MessageQueue.h"
#include "Scheduler.h"



volatile int Scheduler::housekept;

void Scheduler::houseKeep(Message *msg) {
  housekept++;
  if(housekept%10 != 0)
    return;
  systemQueue.send(MessageType::TemperatureRequest,(char) 0);
  systemQueue.send(MessageType::Housekeep,"housekeeping");
}


Scheduler::Scheduler() {
  systemQueue.registerListener(MessageType::OneHz,&houseKeep);
}

