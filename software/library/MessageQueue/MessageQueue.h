#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H
#include <Arduino.h>
#include "config.h"
#include <stddef.h>
#include <avr/pgmspace.h>

#ifndef PAYLOAD_LENGTH
    #error "please define PAYLOAD_LENGTH"
#endif
#ifndef LISTENERS_COUNT_MAX
    #error "please define LISTENERS_COUNT_MAX"
#endif
#ifndef MSG_QUEUE_LENGTH
    #error "please define MSG_QUEUE_LENGTH"
#endif


typedef struct  Message {
    MessageType type;
    char payload[PAYLOAD_LENGTH];
} Message;

struct CommandPayload {
    uint32_t arg1;
    uint32_t arg2;
};

typedef void (*Listener)(Message*);

typedef struct  ListenerEntry {
    MessageType type;
    Listener listener;
} ListenerEntry;


class MessageQueue {
    friend class MessageQueueTest;
    protected:
        Message messageQueue[MSG_QUEUE_LENGTH];
        int messageQueueTail;
        int messageQueueHead;
        ListenerEntry listeners[LISTENERS_COUNT_MAX];
        volatile int messageInProgress = 0;
    public:
        void send(Message *message);
        void send(MessageType type, char value);
        void send(MessageType type, const char* value);
        void send(MessageType type, const __FlashStringHelper* value);
        void send(MessageType type, int value);
        void send(MessageType type, float value);
        Message* receive();
        int registerListener(MessageType type,Listener listener);
        int unregisterListener(MessageType type,Listener listener);
        int dispatch(Message *msg);
};

extern MessageQueue systemQueue;
#endif