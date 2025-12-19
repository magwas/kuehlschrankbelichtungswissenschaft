#include "MCP7940.h"
#include "MessageQueue.h"

enum class ClockCommands : int { get, set, adjust};

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

extern Clock clock;
