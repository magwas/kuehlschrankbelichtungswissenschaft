#include <Arduino.h>

#define LED 14
void setup()
{
    delay(3000);
    pinMode(LED,OUTPUT);
    sensors.begin();
}

void loop()
{
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
}


