#include <Arduino.h>
#include "lamp.h"

void Lamp::setPPF(int ppf65, int ppf18) {
  if(!isLampOn)
    Lamp::on();
  Lamp::setPPF65(ppf65);
  Lamp::setPPF18(ppf18);
}

void Lamp::setlamp(int port, float ppf) {
  int value = ppf*PPF2DUTYCYCLE_MULTIPLIER;
  Serial.print("setting ");
  Serial.print(port);
  Serial.print(" to ");
  Serial.println(ppf);
  Serial.print(" = ");
  Serial.println(value);
  analogWrite(port,value);
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
  for(int i=0;i< 256;i++){
    analogWrite(PWM_MAINS,i);
    delay(20);
  }
  isLampOn = 1;
}

void Lamp::off() {
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

Lamp lamp;