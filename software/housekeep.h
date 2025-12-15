#include "config.h"
#include "MessageQueue.h"

class HouseKeeper {
    protected:
        static volatile int housekept;
        static ListenerEntry houseKeepEntry;
        static void houseKeep(Message * msg);
    public:
        HouseKeeper();
};
