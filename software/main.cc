#include <Arduino.h>
#include "temperature.h"
#include "housekeep.h"
#include "userinterface.h"
#include "NTC_thermistor.h"
#include "button.h"
#include "printf.h"
#include "clock.h"
#include "MessageQueue.h"

unsigned char fill = 0;

void testPWM(Message *msg) {
    if(msg->payload[0]=='0') {
        fill-=10;
    }
    if(msg->payload[0]=='1') {
        fill+=10;
    }
    printf("testpwm: %u\n",fill);
    analogWrite(TEST_PWM,fill*10);
}

ListenerEntry testPWMEntry = {MSG_BUTTON,&testPWM};


void setup()
{
  printf_begin();
  Serial.begin(9600);
  delay(10000);
  Serial.println("hello");
  clockInit();
  temperatureInit();
  houseKeepInit();
  pinMode(A0,OUTPUT);
  initialize_buttons();
  systemQueue.registerListener(&testPWMEntry);
}

void loop()
{
    houseKeep();
    delay(100);
    Message *msg=systemQueue.get();
    while(msg!=NULL) {
        systemQueue.dispatch(msg);
        msg=systemQueue.get();
    }
}


