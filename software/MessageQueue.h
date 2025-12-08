#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <stddef.h>

#define PAYLOAD_LENGTH 16
#define MSG_QUEUE_LENGTH 16

typedef struct  Message {
    int type;
    char payload[PAYLOAD_LENGTH];
} Message;

typedef void (*Listener)(Message*);

typedef struct  ListenerEntry {
    int messageType;
    Listener listener;
    struct ListenerEntry* next;
} ListenerEntry;

class MessageQueue {
    friend class MessageQueueTest;
    protected:
        Message messageQueue[MSG_QUEUE_LENGTH];
        unsigned int messageQueueHead = 0;
        unsigned int messageQueueTail = 0;
        volatile int messageInProgress = 0;
        ListenerEntry* listeners = NULL;
        ListenerEntry* listenersGetTail();
    public:
        void add(Message *message);
        Message* get();
        void registerListener(ListenerEntry* entry);
        void unregisterListener(ListenerEntry * entry);
        void dispatch(Message *msg);
};

extern MessageQueue systemQueue;
#endif