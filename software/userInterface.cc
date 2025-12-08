#include <Arduino.h>
#include "userinterface.h"
#include "lamp.h"
#include "MessageQueue.h"

Message userInput;
volatile char didRead = 0;

void readPrimitive() {
    size_t available = Serial.available();
    if(available > 0) {
        if(didRead+available>PAYLOAD_LENGTH) {
            available = PAYLOAD_LENGTH-didRead;
        }
        Serial.readBytes((char *)&(userInput.payload)+didRead,available);
        didRead+=available;
        char* pos = strchr((char *)&(userInput.payload), '\n');
        *pos=0;
        if(NULL != pos) {
            systemQueue.add(&userInput);
            int left = didRead-(pos-((char*)&(userInput.payload)));
            if(left>0)
                memcpy(&(userInput.payload),pos,left);
            didRead = left;
        }
    }
}

void relayCommand(char * payload) {
    int interval;
    if(1==sscanf(payload, "%*s %u", &interval)) {
        Serial.println(interval);
        digitalWrite(A0,HIGH);
        delay(interval);
        digitalWrite(A0,LOW);
    }
    printf("cannot parse: %s",payload);
}

void setCommand(char * payload) {
    int ppf65,ppf18;
    if(2==sscanf(payload, "%*s %u %u", &ppf65,&ppf18)) {
        lamp_setPPF(ppf65,ppf18);
    } else
    printf("cannot parse: %s",payload);
}

void cmdParser(Message * message) {
    char * payload=message->payload;
    if (!strcmp(payload, "on")) {
        lamp_turnOn();
        Serial.print(" turned on ");
        return;
    }
    if (!strcmp(payload, "off")) {
        lamp_turnOff();
        Serial.print(" turned off");
        return;
    }
    if (!strcmp(payload, "relay ")) {
        relayCommand(payload);
        return;
    }
    if (!strcmp(payload, "set ")) {
        setCommand(payload);
        return;
    }
    printf("cannot parse: %s",payload);
}
