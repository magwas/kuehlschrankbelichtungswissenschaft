#include "Square.h"

int Square::port=-1;
void Square::blip(Message *msg) {
    if(port != -1) {
        digitalWrite(port,!digitalRead(port));
    }
}
void Square::command(Message *msg) {
    CommandPayload *payload = (CommandPayload *)msg->payload;
    pinMode(payload->arg1,OUTPUT);
    port=payload->arg1;
}
Square::Square() {
    systemQueue.registerListener(MessageType::OneHz,&blip);
    systemQueue.registerListener(MessageType::Square,&command);
}

