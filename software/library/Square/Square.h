#include "MessageQueue.h"

class Square {
    protected:
        static int port;
        static unsigned char value;
        static void blip(Message *msg);
    public:
        static void command(Message *msg);
        Square();
};

extern Square tester;


int Square::port=-1;
void Square::blip(Message *msg) {
    if(port != -1) {
        digitalWrite(port,!digitalRead(port));
    }
}
void Square::command(Message *msg) {
    CommandPayload *payload = (CommandPayload *)msg->payload;
    pinMode(payload->arg1,OUTPUT);
    port=(int)payload->arg1;
}
Square::Square() {
    systemQueue.registerListener(MessageType::OneHz,&blip);
    systemQueue.registerListener(MessageType::Square,&command);
}

