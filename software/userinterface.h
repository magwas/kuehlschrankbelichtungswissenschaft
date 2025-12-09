#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "config.h"
#include "MessageQueue.h"

class UserInterface {
    protected:
        static Message userInput;
        static volatile int didRead ;
        static ListenerEntry readLineEntry ;
        static ListenerEntry cmdParserEntry;
        static void readLine(Message *msg);
        static void relayCommand(char * payload);
        static void setCommand(char * payload);
        static void cmdParser(Message * message);
    public:
        UserInterface();
};
#endif