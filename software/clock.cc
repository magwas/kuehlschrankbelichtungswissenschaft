#include "clock.h"

MCP7940_Class     MCP7940;

DateTime Clock::getTime() {
    DateTime now =  MCP7940.now();
    sync(now.unixtime());
    return now;

}

void Clock::formatTime(char *buffer, DateTime now) {
    sprintf(buffer,"%04d-%02d-%02d %02d:%02d:%02d",
            now.year(),
            now.month(), now.day(), now.hour(), now.minute(),
            now.second());
}

void Clock::printTime() {
    DateTime now = getTime();
    char buffer[PAYLOAD_LENGTH];
    formatTime((char *)buffer,now);
    systemQueue.send(MessageType::Console,buffer);
}

void Clock::settTime(DateTime time) {
    MCP7940.adjust(time);
    sync(time.unixtime());
}

int8_t Clock::calibrateTime(DateTime time) {
    sync(time.unixtime());
    return MCP7940.calibrate(time);
}

DateTime Clock::parseTime(char *timeString) {
    short unsigned int tokens, year, month, day, hour, minute, second;
    tokens = sscanf(timeString,
                          "%hu-%hu-%hu %hu:%hu:%hu",
                          &year, &month, &day, &hour, &minute, &second);
    if (tokens != 6)
        return 0;
    return DateTime(year, month, day,hour, minute, second);
}

void Clock::begin() {
    while (!MCP7940.begin()) {
        systemQueue.send(MessageType::Console,F("no MCP7940. recheck in 3s."));
        delay(3000);
    }
    systemQueue.send(MessageType::Console,F("MCP7940 initialized."));
    while (!MCP7940.deviceStatus()) {
        systemQueue.send(MessageType::Console,F("Oscillator is off, turning it on."));
        bool deviceStatus = MCP7940.deviceStart();
        if (!deviceStatus) {
            systemQueue.send(MessageType::Console,F("Oscillator did not start, trying again."));
            delay(1000);
        }
    }
  MCP7940.setSQWState(true);
  MCP7940.setSQWSpeed(Hz1);
}

void Clock::sync(uint32_t unixtime) {
    syncUnix = unixtime;
    syncMillis = millis();
}

uint32_t Clock::fastUnixtime() {
    unsigned long currentMillis = millis();
    unsigned long elapsedMillis = currentMillis - syncMillis;
    return syncUnix + (elapsedMillis / 1000UL);
}

Clock clock;
