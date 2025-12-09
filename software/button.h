#include "config.h"
#include "MessageQueue.h"

class Buttons {
    protected:
        static Message button1Message;
        static Message button2Message;
        static void interrupt_butt1();

        static void interrupt_butt2();
    public:
        Buttons();
};
