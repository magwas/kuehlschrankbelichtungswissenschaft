#include <Arduino.h>
#include "userinterface.h"
#include "lamp.h"
#include "MessageQueue.h"

Message UserInterface::userInput={MessageType::Command,""};
volatile int UserInterface::didRead = 0;


void UserInterface::readLine(Message *msg) {
    char received = msg->payload[0];
    if(13 == received) {
        userInput.payload[didRead]=0;
        systemQueue.send(&userInput);
        didRead = 0;
        return;
    }
    userInput.payload[didRead]=received;
    didRead++;
    if(didRead>=PAYLOAD_LENGTH-1)
    didRead = 0;
}


void UserInterface::relayCommand(char * payload) {
    int interval;
    int tokens =sscanf(payload, "%*s %u\n", &interval);
    if(1==tokens) {
        Serial.println(interval);
        digitalWrite(A0,HIGH);
        delay(interval);
        digitalWrite(A0,LOW);
        return;
    }
    printf("cannot parse: %s (%u)\n",payload, tokens);
}

void UserInterface::setCommand(char * payload) {
    int ppf65,ppf18;
    if(2==sscanf(payload, "%*s %u %u", &ppf65,&ppf18)) {
        lamp.setPPF(ppf65,ppf18);
    } else
    printf("cannot parse: %s",payload);
}


void UserInterface::cmdParser(Message * message) {
    char * payload=message->payload;
    printf("cmd: %s\n",payload);

    if (!strcmp(payload, "on")) {
        lamp.on();
        Serial.print(" turned on ");
        return;
    }
    if (!strcmp(payload, "off")) {
        lamp.off();
        Serial.print(" turned off");
        return;
    }
    if (!strncmp(payload, "relay ",6)) {
        relayCommand(payload);
        return;
    }
    if (!strncmp(payload, "set ",4)) {
        setCommand(payload);
        return;
    }
    printf("cannot parse: %s",payload);
}

UserInterface::UserInterface(){
    systemQueue.registerListener(MessageType::Serial,&readLine);
    systemQueue.registerListener(MessageType::Command,&cmdParser);
}
