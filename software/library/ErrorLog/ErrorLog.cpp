#include <MCP7940.h>
#include <EEPROM.h>
#include "ErrorLog.h"
#include "Clock.h"

void ErrorLog::printAll() {
    char buffer[PAYLOAD_LENGTH];
    char buffer2[PAYLOAD_LENGTH];
    for(int i = 0; i<MAX_ERRORS ; i++) {
        ErrorMessage msg = read(i);
        DateTime time = DateTime(msg.time);
        clock.formatTime((char *)buffer,time);
        sprintf(buffer2,"%s %u %u",buffer,(int)msg.errorCode,msg.additionalData);
        systemQueue.send(MessageType::Console,buffer2);
    }
}

void ErrorLog::write(char code, int data) {
    int nextWritePos = findOldestMessageIndex();
    ErrorMessage newMsg;
    newMsg.time = clock.fastUnixtime();
    newMsg.errorCode = code;
    newMsg.additionalData = data;

    uint16_t addr = nextWritePos * sizeof(ErrorMessage);
    EEPROM.put(addr, newMsg);
}

int ErrorLog::findOldestMessageIndex() {
    int oldestIndex = 0;
    uint32_t oldestTime = -1;
    for(uint16_t i = 0; i < MAX_ERRORS; i++) {
        uint16_t addr = i * sizeof(ErrorMessage);
        uint32_t thisTime;
        EEPROM.get(addr, thisTime);
        if(thisTime<oldestTime) {
            oldestIndex = i;
            oldestTime = thisTime;
        }
    }
    return oldestIndex;
}

ErrorMessage ErrorLog::read(uint16_t index) {
    ErrorMessage msg;
    if(index >= MAX_ERRORS)
        return msg;
    uint16_t addr = index * sizeof(ErrorMessage);
    EEPROM.get(addr, msg);
    return msg;
}

void ErrorLog::clean() {
    uint32_t thisTime = 0;
    for(uint16_t i = 0; i < MAX_ERRORS; i++) {
        uint16_t addr = i * sizeof(ErrorMessage);
        EEPROM.put(addr, thisTime);
    }

}

void errorCommand(Message *msg) {
    CommandPayload *payload = (CommandPayload *)msg->payload;
    errorLog.write(payload->arg1,payload->arg2);
    char buffer[PAYLOAD_LENGTH];
    sprintf(buffer,"ERROR code=%u, arg=%u",(char)payload->arg1,(int)payload->arg2);
    systemQueue.send(MessageType::Console,buffer);
}

void errorsCommand(Message *msg) {
    CommandPayload *payload = (CommandPayload *)msg->payload;
    switch(payload->arg1) {
        case 0:
            errorLog.printAll();
            break;
        case 1:
            errorLog.clean();
            break;
        default:
            systemQueue.send(MessageType::Console,F("0: print all, 1: clean"));
    }
}

ErrorLog::ErrorLog() {
    systemQueue.registerListener(MessageType::Error,&errorCommand);
    systemQueue.registerListener(MessageType::Errors,&errorsCommand);
}

