#include <Arduino.h>

#define LED LED_BUILTIN
void setup()
{
    pinMode(LED,OUTPUT);
}

void loop()
{
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
}


