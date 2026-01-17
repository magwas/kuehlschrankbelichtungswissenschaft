#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H
#include <Arduino.h>
#include <config.h>
#include <stddef.h>

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
    alignas(uint32_t) char payload[PAYLOAD_LENGTH];
} Message;

struct CommandPayload {
    uint32_t arg1;
    uint32_t arg2;
    uint32_t arg3;
};
struct TemperaturePayload {
    float temp;
    uint32_t index;
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
    public:
        void send(Message *message);
        void send(MessageType type, char value);
        void send(MessageType type, const char* value);
        void send(MessageType type, const __FlashStringHelper* value);
        void send(MessageType type, int value);
        void send(MessageType type, float value,int index);
        void printf(const char* fmt, ...);
        Message* receive();
        int registerListener(MessageType type,Listener listener);
        int unregisterListener(MessageType type,Listener listener);
        int dispatch(Message *msg);
};

extern MessageQueue systemQueue;

#include <string.h>


void MessageQueue::send(Message *messageP) {
    noInterrupts();
    int newTail = (messageQueueTail + 1) % MSG_QUEUE_LENGTH;
    if (newTail == messageQueueHead) {
        interrupts();
        return; // Queue full - drop message
    }
    int oldTail = messageQueueTail;
    messageQueueTail = (messageQueueTail+1)%MSG_QUEUE_LENGTH;
    memcpy(&(messageQueue[oldTail]),messageP,sizeof(Message));
    interrupts();
}

void MessageQueue::send(MessageType type, char value) {
    noInterrupts();
    int oldTail = messageQueueTail;
    messageQueueTail = (messageQueueTail+1)%MSG_QUEUE_LENGTH;
    messageQueue[oldTail].type=type;
    messageQueue[oldTail].payload[0]=value;
    interrupts();
}

void MessageQueue::send(MessageType type, const char* value) {
    noInterrupts();
    int oldTail = messageQueueTail;
    messageQueueTail = (messageQueueTail+1)%MSG_QUEUE_LENGTH;
    messageQueue[oldTail].type=type;
    memcpy(messageQueue[oldTail].payload,value,PAYLOAD_LENGTH);
    interrupts();
}

void MessageQueue::printf(const char* fmt,...) {
    noInterrupts();
    char buffer[PAYLOAD_LENGTH];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer,PAYLOAD_LENGTH, fmt, args);
    va_end(args);
    int oldTail = messageQueueTail;
    messageQueueTail = (messageQueueTail+1)%MSG_QUEUE_LENGTH;
    messageQueue[oldTail].type=MessageType::Console;
    memcpy(messageQueue[oldTail].payload,buffer,PAYLOAD_LENGTH);
    interrupts();
}

void MessageQueue::send(MessageType type, const __FlashStringHelper* value) {
    noInterrupts();
    int oldTail = messageQueueTail;
    messageQueueTail = (messageQueueTail+1)%MSG_QUEUE_LENGTH;
    messageQueue[oldTail].type=type;
    memcpy_P(messageQueue[oldTail].payload,value,PAYLOAD_LENGTH);
    interrupts();
}

void MessageQueue::send(MessageType type, float value, int index) {
    noInterrupts();
    int oldTail = messageQueueTail;
    messageQueueTail = (messageQueueTail+1)%MSG_QUEUE_LENGTH;
    messageQueue[oldTail].type=type;
    TemperaturePayload *payload = ((TemperaturePayload*) (messageQueue[oldTail].payload));
    payload->temp = value;
    payload->index = index;
    interrupts();
}

void MessageQueue::send(MessageType type, int value) {
    noInterrupts();
    int oldTail = messageQueueTail;
    messageQueueTail = (messageQueueTail+1)%MSG_QUEUE_LENGTH;
    messageQueue[oldTail].type=type;
    ((int *)messageQueue[oldTail].payload)[0]=value;
    interrupts();
}

Message* MessageQueue::receive() {
    noInterrupts();
    if(messageQueueTail == messageQueueHead) {
        interrupts();
        return NULL;
    }
    int oldHead = messageQueueHead;
    messageQueueHead = (messageQueueHead+1)%MSG_QUEUE_LENGTH;
    interrupts();
    return &(messageQueue[oldHead]);
}

int MessageQueue::registerListener(MessageType type,Listener listener) {
    noInterrupts();
    for(int i=0;i<LISTENERS_COUNT_MAX;i++) {
        if(listeners[i].type==MessageType::None) {
            listeners[i].type = type;
            listeners[i].listener = listener;
            interrupts();
            return i;
        }
    }
    interrupts();
    return -1;
}

int MessageQueue::unregisterListener(MessageType type,Listener listener) {
    noInterrupts();
    for(int i=0;i<LISTENERS_COUNT_MAX;i++) {
        if(listeners[i].type==type && listeners[i].listener == listener) {
            listeners[i].type = MessageType::None;
            interrupts();
            return i;
        }
    }
    interrupts();
    return -1;
}
int MessageQueue::dispatch(Message *msg) {
    int count = 0;
    for(int i=0;i<LISTENERS_COUNT_MAX;i++) {
        if(listeners[i].type == MessageType::ALL || listeners[i].type==msg->type) {
            (*(listeners[i].listener))(msg);
            count++;
        }
    }
    return count;
}



#endif
