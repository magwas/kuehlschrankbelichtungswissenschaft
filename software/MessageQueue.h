#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H
#include "config.h"
#include <stddef.h>

#define PAYLOAD_LENGTH 16
#define LISTENERS_COUNT_MAX 16
#define MSG_QUEUE_LENGTH 16


typedef struct  Message {
    MessageType type;
    char payload[PAYLOAD_LENGTH];
} Message;

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
        void send(MessageType type, int value);
        void send(MessageType type, float value);
        Message* receive();
        int registerListener(MessageType type,Listener listener);
        int unregisterListener(MessageType type,Listener listener);
        int dispatch(Message *msg);
};

extern MessageQueue systemQueue;
#endif