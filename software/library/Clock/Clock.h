#include "MCP7940.h"
#include "MessageQueue.h"

enum class ClockCommands : int { get, set, adjust, status};

enum SquareWaveTypes { Hz1, kHz4, kHz8, kHz32, Hz64 };

class Clock {
    protected:
        static void interrupt_1S();
        uint32_t syncUnix;
        unsigned long syncMillis;
        void sync(uint32_t unixtime);
        static void command(Message *msg);
    public:
        Clock();
        void begin();
        uint32_t fastUnixtime();
        DateTime getTime();
        void printTime();
        void setTime(DateTime time);
        int8_t calibrateTime(DateTime time);
        DateTime parseTime(char *timeString);
        void formatTime(char *buffer, DateTime now);
};

extern Clock rtc;


#ifndef INT_1S
    #error "please define INT_1S as pin number for 1HZ signal"
#endif

MCP7940_Class     MCP7940;

void ICACHE_RAM_ATTR Clock::interrupt_1S() {
    systemQueue.send(MessageType::OneHz,(char)0);
}

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

void Clock::setTime(DateTime time) {
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

Clock::Clock() {
    systemQueue.send(MessageType::Console,F("initializing clock"));
}
void Clock::begin() {
    Serial.println("begin");
    while (!MCP7940.begin()) {
        systemQueue.send(MessageType::Console,F("no MCP7940. recheck in 3s."));
        delay(3000);
    }
    Serial.println("got");
    systemQueue.send(MessageType::Console,F("MCP7940 initialized."));
    Serial.println("sent");
    while (!MCP7940.deviceStatus()) {
        systemQueue.send(MessageType::Console,F("Oscillator is off, turning it on."));
        bool deviceStatus = MCP7940.deviceStart();
        if (!deviceStatus) {
            systemQueue.send(MessageType::Console,F("Oscillator did not start, trying again."));
            delay(1000);
        }
    }
    Serial.println("sent");
    MCP7940.setSQWState(true);
    MCP7940.setSQWSpeed(Hz1);
    MCP7940.setBattery(true);
    Serial.println("bat");
    if (!MCP7940.getBattery()) {
    systemQueue.send(MessageType::Console,"no bat backup");
    }
    pinMode(INT_1S, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(INT_1S), interrupt_1S, FALLING);
    systemQueue.registerListener(MessageType::Clock,&command);
    Serial.println("clock started");
}

void Clock::command(Message *msg) {
    CommandPayload *payload = (CommandPayload *)msg->payload;
    DateTime time = DateTime(payload->arg2);
    char buffer[PAYLOAD_LENGTH];
    int8_t calibration;

    switch(static_cast<ClockCommands>(payload->arg1)) {
        uint8_t speed ;
        bool state ;
        bool bat;
        bool clock;

        case ClockCommands::get:
            rtc.printTime();
            break;
        case ClockCommands::set:
            rtc.setTime(time);
            break;
        case ClockCommands::adjust:
            calibration = rtc.calibrateTime(time);
            sprintf(buffer,"calibration: %d",calibration);
            systemQueue.send(MessageType::Console,buffer);
            break;
        case ClockCommands::status:
            speed = MCP7940.getSQWSpeed();
            state = MCP7940.getSQWState ();
            bat =	MCP7940.getBattery ();
            clock = MCP7940.clockStatus();
            systemQueue.printf("speed=%u,sqw=%d,bat=%d,clk=%u",(int)speed,state,bat,clock);
            break;
        default:
            systemQueue.send(MessageType::Console,"0: r, 1: w, 2: cal, 3 stat");
    }
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

