#include "MCP7940.h"
#include "MessageQueue.h"

enum SquareWaveTypes { Hz1, kHz4, kHz8, kHz32, Hz64 };
class Clock {
    protected:
        uint32_t syncUnix;
        unsigned long syncMillis;
        void sync(uint32_t unixtime);
    public:
        uint32_t fastUnixtime();
        void begin();
        DateTime getTime();
        void printTime();
        void settTime(DateTime time);
        int8_t calibrateTime(DateTime time);
        DateTime parseTime(char *timeString);
        void formatTime(char *buffer, DateTime now);
};

extern Clock clock;
