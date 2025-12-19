#include "MessageQueue.h"

class Adc {
        static char channels[];
        static void command(Message * msg);
    public:
        static int set(char channel, char value);
        Adc();
};

extern Adc adc;