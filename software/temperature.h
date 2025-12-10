#include "config.h"
#include "MessageQueue.h"

class Thermometer {
    protected:
        static void read(Message * msg);
    public:
        Thermometer();
};

extern Thermometer thermometer;