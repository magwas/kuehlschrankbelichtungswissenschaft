#include <Arduino.h>
#include "config.h"
#include "button.h"

void Buttons::interrupt_butt1() {
    systemQueue.send(MessageType::Button,(char)0);
}

void Buttons::interrupt_butt2() {
    systemQueue.send(MessageType::Button,(char)1);
}
Buttons::Buttons() {
      pinMode(BUTTON_1,INPUT_PULLUP);
      attachInterrupt(digitalPinToInterrupt(BUTTON_1), interrupt_butt1, FALLING);
      pinMode(BUTTON_2,INPUT_PULLUP);
      attachInterrupt(digitalPinToInterrupt(BUTTON_2), interrupt_butt2, FALLING);
}

