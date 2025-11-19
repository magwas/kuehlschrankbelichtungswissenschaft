#include <Arduino.h>
#include "lamp.h"

char isLampOn = 0;

void lamp_setPPF(int ppf65, int ppf18) {
  if(!isLampOn)
    lamp_turnOn();
  lamp_setPPF65(ppf65);
  lamp_setPPF18(ppf18);
}

#define PPF65_MAX 1000
#define PPF18_MAX 800
#define PPF2DUTYCYCLE_MULTIPLIER (0.0612f)
#define PPF_MIN 130

void setlamp(int port, float ppf) {
  Serial.print("setting ");
  Serial.print(port);
  Serial.print(" to ");
  Serial.println(ppf);
  analogWrite(port,ppf*PPF2DUTYCYCLE_MULTIPLIER);
}

void lamp_setPPF18(int ppf18) {
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


void lamp_setPPF65(int ppf65) {
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

void lamp_turnOn() {
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

void lamp_turnOff() {
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
