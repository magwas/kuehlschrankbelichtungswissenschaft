#include "Adc.h"

char Adc::channels[] = ADC_CHANNELS;
int Adc::set(char channel, char value) {
    if(channel < (char)sizeof(channels)) {
        analogWrite(channels[channel],value);
        return channel;
    }
    return -1;
}

void Adc::command(Message *msg) {
    char buffer[PAYLOAD_LENGTH];
    CommandPayload *payload = (CommandPayload *)msg->payload;
    if(-1 == set(payload->arg1,payload->arg2)) {
        sprintf(buffer,"no Adc %u",(int)payload->arg1);
        systemQueue.send(MessageType::Console,buffer);
    }else {
        sprintf(buffer,"Adc %u %u",channels[payload->arg1],payload->arg2);
        systemQueue.send(MessageType::Console,buffer);
    }
}

Adc::Adc() {
    systemQueue.registerListener(MessageType::PWM,&command);
}

