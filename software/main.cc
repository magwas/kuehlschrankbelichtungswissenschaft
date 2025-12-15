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

char buffer[PAYLOAD_LENGTH];

void testPWM(Message *msg) {
    if(msg->payload[0]=='v') {
        fill-=10;
    }
    if(msg->payload[0]=='^') {
        fill+=10;
    }
    sprintf(buffer,"testpwm: %u  ",fill);
    systemQueue.send(MessageType::Console, buffer);
    analogWrite(TEST_PWM,fill);
}

void tempread(Message *msg) {
    float *floatp = (float *)msg->payload;
    float val = *floatp;
    sprintf(buffer,"%.2f C  ",val);
    systemQueue.send(MessageType::Console, buffer);
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
  errorLog.write('s',INT_VERSION);
  pinMode(A0,OUTPUT);
  systemQueue.registerListener(MessageType::Button,&testPWM);
  systemQueue.registerListener(MessageType::Temperature,&tempread);
  int n = systemQueue.registerListener(MessageType::Console,&console);
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


