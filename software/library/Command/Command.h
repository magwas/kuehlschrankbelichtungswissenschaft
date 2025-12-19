#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "config.h"
#include "MessageQueue.h"

struct CommandParams {
    MessageType type;
    uint8_t args;
};

struct CommandEntry {
    const char* name;
    CommandParams params;
};

class Command {
    protected:
        static Message userInput;
        static volatile int didRead;
        static void readLine(Message *msg);
        static void relayCommand(char * payload);
        static void cmdParser(Message * message);
        static CommandParams commandToMessageType(const char*);
        static void help();
    public:
        Command();
};

extern Command cmd;
#endif