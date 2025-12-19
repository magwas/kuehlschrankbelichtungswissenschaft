#include <Arduino.h>
#include <avr/pgmspace.h>
#include "Command.h"
#include "MessageQueue.h"

#ifndef COMMAND_TABLE
    #error "please define COMMAND_TABLE with some ENTRY(string,type,#args)"
#endif

#define ENTRY(str, type, args) const char cmd_##str##_str[] PROGMEM = #str;
COMMAND_TABLE
#undef ENTRY

const CommandEntry commandTable[] PROGMEM = {
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
    char command[PAYLOAD_LENGTH];
    int tokens =sscanf(payload, "%s %lu %lu\n",command, &arg1, &arg2);
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
    }
    systemQueue.send(&msg);

}

Command::Command(){
    systemQueue.registerListener(MessageType::Serial,&readLine);
    systemQueue.registerListener(MessageType::Command,&cmdParser);
}

