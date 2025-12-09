#include "config.h"
#include "MessageQueue.h"

class HouseKeeper {
    protected:
        static volatile int housekept;
        static Message oneHzMessage;
        static ListenerEntry houseKeepEntry;
        static void interrupt_1S();
        static void houseKeep(Message * msg);
    public:
        HouseKeeper();
};
