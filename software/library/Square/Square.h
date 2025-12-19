#include "MessageQueue.h"

class Square {
    protected:
        static int port;
        static void blip(Message *msg);
    public:
        static void command(Message *msg);
        Square();
};

extern Square fff;