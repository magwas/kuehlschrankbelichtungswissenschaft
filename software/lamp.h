#include "config.h"

#define PPF65_MAX 1000
#define PPF18_MAX 800
#define PPF2DUTYCYCLE_MULTIPLIER (0.0612f*10)
#define PPF_MIN 130

class Lamp {
    protected:
        char isLampOn=0;
        void setlamp(int port, float ppf);
        void setPPF18(int ppf18);
        void setPPF65(int ppf65);
    public:
        void setPPF(int ppf65, int ppf18);
        void on();
        void off();
};

extern Lamp lamp;