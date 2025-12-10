#include <MCP7940.h>
#include <EEPROM.h>
#include "ErrorLog.h"
#include "clock.h"

void ErrorLog::printAll() {
    for(int i = 0; i<MAX_ERRORS ; i++) {
        ErrorMessage msg = read(i);
        Serial.print(i);
        Serial.print(" ");
        DateTime time = DateTime(msg.time);
        char buffer[PAYLOAD_LENGTH];
        clock.formatTime((char *)buffer,time);
        Serial.print(buffer);
        Serial.print(" ");
        Serial.print(msg.errorCode);
        Serial.print(" ");
        Serial.println(msg.additionalData);
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

ErrorLog errorLog;