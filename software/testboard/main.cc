#include <Arduino.h>
#include "version.h"
#include "Scheduler.h"
#include "Command.h"
//#include "NTC_thermistor.h"
#include "Clock.h"
#include "MessageQueue.h"
#include "Thermometer.h"
#include "Buttons.h"
#include "ErrorLog.h"
#include "Adc.h"
#include "Lamp.h"
#include "Square.h"


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
    CommandPayload *args = (CommandPayload *)buffer;
    args->arg1=2;
    args->arg2=fill;
    systemQueue.send(MessageType::PWM,buffer);
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

MessageQueue systemQueue;
Command cmd;
ErrorLog errorLog;
Scheduler scheduler;
Clock clock;
Adc adc;
Buttons buttons;
Lamp lamp;
Square squareWawe;
Thermometer thermometer;

void setup()
{
  delay(3000);
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
  Serial.println(n);
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


