#ifndef COMMAND_H
#define COMMAND_H

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

#include <Arduino.h>

#ifndef COMMAND_TABLE
    #error "please define COMMAND_TABLE with some ENTRY(string,type,#args)"
#endif

#define ENTRY(str, type, args) const char cmd_##str##_str[] = #str;
COMMAND_TABLE
#undef ENTRY

const CommandEntry commandTable[] = {
#define ENTRY(str, type, args) {cmd_##str##_str, {MessageType::type, args}},
COMMAND_TABLE
#undef ENTRY
};

Message Command::userInput={MessageType::Command,""};
volatile int Command::didRead =0;


void Command::readLine(Message *msg) {
    char received = msg->payload[0];
    if(13 == received) {
        userInput.payload[didRead]=0;
        systemQueue.send(&userInput);
        didRead = 0;
        return;
    }
    userInput.payload[didRead]=received;
    didRead++;
    if(didRead>=PAYLOAD_LENGTH-1)
    didRead = 0;
}

#define TABLE_SIZE  (sizeof(commandTable) / sizeof(CommandEntry))

CommandParams Command::commandToMessageType(const char* command) {
    for (uint8_t i = 0; i < TABLE_SIZE; i++) {
        const char* namePtr = (const char*)pgm_read_ptr(&commandTable[i].name);
        int iseq = strcmp_P(command, namePtr);
        if ( iseq == 0) {
            CommandParams params;
            params.type = static_cast<MessageType>(pgm_read_byte(
                &commandTable[i].params.type));
            params.args = pgm_read_byte(&commandTable[i].params.args);
            return params;
        }
    }
    return CommandParams{MessageType::None, 0};
}

void Command::help() {
#define ENTRY(cmd,msg,args) systemQueue.send(MessageType::Console,F(#cmd ":" #args " args"));
COMMAND_TABLE
#undef ENTRY
}
void Command::cmdParser(Message * message) {
    char * payload=message->payload;
    uint32_t arg1;
    uint32_t arg2;
    uint32_t arg3;
    char command[PAYLOAD_LENGTH];
    int tokens =sscanf(payload, "%s %lu %lu %lu\n",command, &arg1, &arg2, &arg3);
    CommandParams params = commandToMessageType(command);
    Message msg;
    msg.type=MessageType::Console;
    if(params.type == MessageType::None) {
        help();
        return;
    } else if(tokens != params.args +1) {
        sprintf(msg.payload,"need %u args: %s",params.args,payload);
    } else {
        msg.type = params.type;
        CommandPayload *payload = (CommandPayload *)msg.payload;
        payload->arg1 = arg1;
        payload->arg2 = arg2;
        payload->arg3 = arg3;
    }
    systemQueue.send(&msg);

}

Command::Command(){
		systemQueue.send(MessageType::Console,"Command initialized");
    systemQueue.registerListener(MessageType::Serial,&readLine);
    systemQueue.registerListener(MessageType::Command,&cmdParser);
}

#endif
