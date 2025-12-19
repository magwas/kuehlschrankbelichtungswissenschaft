#include <Arduino.h>
#include "Lamp.h"
#include "Adc.h"

void Lamp::setPPF(int ppf65, int ppf18) {
  if(!isLampOn)
    Lamp::on();
  Lamp::setPPF65(ppf65);
  Lamp::setPPF18(ppf18);
}

void Lamp::setlamp(int port, float ppf) {
  int value = ppf*PPF2DUTYCYCLE_MULTIPLIER;
  char buffer[PAYLOAD_LENGTH];
  sprintf(buffer,"setting %u to %f (%u)",port,(double)ppf,value);
  systemQueue.send(MessageType::Console,buffer);
  CommandPayload payload={(uint32_t)port,(uint32_t)value};
  systemQueue.send(MessageType::PWM,(char *)&payload);
}

void Lamp::setPPF18(int ppf18) {
  if(ppf18>PPF18_MAX)
    ppf18 = PPF18_MAX;
  if(ppf18>=PPF_MIN*4) {
    float perlamp = ppf18/4.0;
      setlamp(PWM_18_1,perlamp);
      setlamp(PWM_18_2,perlamp);
  } else {
      setlamp(PWM_18_1,ppf18/2.0);
      setlamp(PWM_18_2,0);
  }
}


void Lamp::setPPF65(int ppf65) {
  if(ppf65>PPF65_MAX)
    ppf65 = PPF65_MAX;
  if(ppf65>=PPF_MIN*5) {
    float perlamp = ppf65/5.0;
      setlamp(PWM_65_1,perlamp);
      setlamp(PWM_65_2,perlamp);
      setlamp(PWM_65_3,perlamp);
  } else if(ppf65>=PPF_MIN*4) {
    float perlamp = ppf65/4.0;
      setlamp(PWM_65_1,perlamp);
      setlamp(PWM_65_2,perlamp);
      setlamp(PWM_65_3,0);
  } else if(ppf65>=PPF_MIN*3) {
    float perlamp = ppf65/3.0;
      setlamp(PWM_65_1,perlamp);
      setlamp(PWM_65_2,0);
      setlamp(PWM_65_3,perlamp);
  } else if(ppf65>=PPF_MIN*2) {
    float perlamp = ppf65/2.0;
      setlamp(PWM_65_1,perlamp);
      setlamp(PWM_65_2,0);
      setlamp(PWM_65_3,0);
  } else  {
      setlamp(PWM_65_1,0);
      setlamp(PWM_65_2,0);
      setlamp(PWM_65_3,ppf65);
  }
}

void Lamp::on() {
  analogWrite(PWM_65_1,0);
  analogWrite(PWM_65_2,0);
  analogWrite(PWM_65_3,0);
  analogWrite(PWM_18_1,0);
  analogWrite(PWM_18_2,0);
  digitalWrite(LAMP_RELAY,HIGH);
  for(int i=0;i< 256;i++){
    analogWrite(PWM_MAINS,i);
    delay(20);
  }
  isLampOn = 1;
}

void Lamp::off() {
  digitalWrite(LAMP_RELAY,LOW);
  for(int i=255;i>=0;i--){
    analogWrite(PWM_MAINS,i);
    delay(20);
  }
  delay(1000);

  analogWrite(PWM_65_1,0);
  analogWrite(PWM_65_2,0);
  analogWrite(PWM_65_3,0);
  analogWrite(PWM_18_1,0);
  analogWrite(PWM_18_2,0);
  isLampOn = 0;
}

void Lamp::setCommand(Message *msg) {
    CommandPayload *payload = (CommandPayload *)msg->payload;
    lamp.setPPF(payload->arg1,payload->arg2);
}

void Lamp::relayCommand(Message *msg) {
    CommandPayload *payload = (CommandPayload *)msg->payload;
    int length = payload->arg1;
    switch(length) {
        case 0:
            digitalWrite(LAMP_RELAY,LOW);
            break;
        case 1:
            digitalWrite(LAMP_RELAY,HIGH);
            break;
        default:
            digitalWrite(LAMP_RELAY,HIGH);
            delay(payload->arg1);
            digitalWrite(LAMP_RELAY,LOW);

    }
}

void Lamp::onOffCommand(Message *msg) {
    CommandPayload *payload = (CommandPayload *)msg->payload;
    switch(payload->arg1) {
        case 0:
            lamp.off();
            break;
        case 1:
            lamp.on();
            break;
        default:
            systemQueue.send(MessageType::Console,"0: off, 1:on");
    }
}

Lamp::Lamp() {
    systemQueue.registerListener(MessageType::Set,&setCommand);
    systemQueue.registerListener(MessageType::Relay,&relayCommand);
    systemQueue.registerListener(MessageType::Lamp,&onOffCommand);
}
