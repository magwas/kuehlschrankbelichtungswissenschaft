#include "clock.h"

MCP7940_Class     MCP7940;

DateTime Clock::getTime() {
    return MCP7940.now();
}

void Clock::printTime() {
    DateTime now = MCP7940.now();
    printf("%04d-%02d-%02d %02d:%02d:%02d\n",
            now.year(),
            now.month(), now.day(), now.hour(), now.minute(),
            now.second());

}

void Clock::settTime(DateTime time) {
    MCP7940.adjust(time);
}

int8_t Clock::calibrateTime(DateTime time) {
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
      Serial.println(F("Unable to find MCP7940. Checking again in 3s."));
      delay(3000);
    }
    Serial.println(F("MCP7940 initialized."));
    while (!MCP7940.deviceStatus()) {
      Serial.println(F("Oscillator is off, turning it on."));
      bool deviceStatus = MCP7940.deviceStart();
      if (!deviceStatus) {
        Serial.println(F("Oscillator did not start, trying again."));
        delay(1000);
      }
    }
  MCP7940.setSQWState(true);
  MCP7940.setSQWSpeed(Hz1);

}