#include <Arduino.h>
#include "temperature.h"
#include "housekeep.h"
#include "userinterface.h"
#include "NTC_thermistor.h"
#include "button.h"
#include "printf.h"
#include "clock.h"
#include "MessageQueue.h"
#include "userinterface.h"

unsigned char fill = 0;


void testPWM(Message *msg) {
    if(msg->payload[0]==0) {
        fill-=10;
    }
    if(msg->payload[0]==1) {
        fill+=10;
    }
    printf("testpwm: %u\n",fill);
    analogWrite(TEST_PWM,fill*10);
}

Buttons buttons;
HouseKeeper houseKeeper;
UserInterface ui;
Clock clock;
Thermometer thermometer;

void setup()
{
  printf_begin();
  Serial.begin(9600);
  clock.begin();
  delay(3000);
  Serial.println("hello");
  clock.printTime();
  pinMode(A0,OUTPUT);
  systemQueue.registerListener(MessageType::Button,&testPWM);
}

Message serialMsg = {MessageType::Serial, 'a'};

void loop()
{
    if(Serial.available()) {
        char read = Serial.read();
        systemQueue.send(MessageType::Serial, read);
    }
    Message *msg=systemQueue.receive();
    while(msg!=NULL) {
        systemQueue.dispatch(msg);
        msg=systemQueue.receive();
    }
}


