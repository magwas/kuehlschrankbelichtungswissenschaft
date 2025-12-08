#include <Arduino.h>
#include "button.h"
#include "MessageQueue.h"
volatile int lastmillis=0;

Message button1Message = {MSG_BUTTON,"0"};
Message button2Message = {MSG_BUTTON,"1"};

void interrupt_butt1() {
    systemQueue.add(&button1Message);
}

void interrupt_butt2() {
    systemQueue.add(&button2Message);
}


void initialize_buttons() {
      pinMode(BUTTON_1,INPUT_PULLUP);
      attachInterrupt(digitalPinToInterrupt(BUTTON_1), interrupt_butt1, FALLING);
      pinMode(BUTTON_2,INPUT_PULLUP);
      attachInterrupt(digitalPinToInterrupt(BUTTON_2), interrupt_butt2, FALLING);
}
