#include "MCP7940.h"

enum SquareWaveTypes { Hz1, kHz4, kHz8, kHz32, Hz64 };
class Clock {
    public:
    void begin();
    DateTime getTime();
    void printTime();
    void settTime(DateTime time);
    int8_t calibrateTime(DateTime time);
    DateTime parseTime(char *timeString);
};

extern Clock clock;
