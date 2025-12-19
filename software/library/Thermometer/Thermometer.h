#include "config.h"
#include "MessageQueue.h"

class Thermometer {
    protected:
        static void read(Message * msg);
    public:
        void begin();
};

extern Thermometer thermometer;