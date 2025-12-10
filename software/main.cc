#include <Arduino.h>
#include "version.h"
#include "housekeep.h"
#include "userinterface.h"
//#include "NTC_thermistor.h"
#include "printf.h"
#include "clock.h"
#include "MessageQueue.h"
#include "temperature.h"
#include "button.h"
#include "ErrorLog.h"

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

void tempread(Message *msg) {
    float *floatp = (float *)msg->payload;
    float val = *floatp;
    Serial.print(val);
    Serial.println(" C");
}

void console(Message * msg) {
    Serial.println(msg->payload);
}

void setup()
{
  delay(3000);
  printf_begin();
  Serial.begin(9600);
  Serial.setTimeout(100);
  Serial.println("Hello!");
  clock.begin();
  thermometer.begin();
  buttons.begin();
  clock.printTime();
  errorLog.printAll();
  errorLog.write('s',INT_VERSION);
  pinMode(A0,OUTPUT);
  systemQueue.registerListener(MessageType::Button,&testPWM);
  systemQueue.registerListener(MessageType::Temperature,&tempread);
  int n = systemQueue.registerListener(MessageType::Console,&console);
  printf("Datetime size: %u",sizeof(DateTime));
  printf("listeners: %u\n",n);

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


