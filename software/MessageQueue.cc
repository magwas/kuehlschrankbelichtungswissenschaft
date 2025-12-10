#include "MessageQueue.h"
#include <string.h>

void spinlock_enter(volatile int *lock) {
    (*lock)++;
    while(1!=(*lock));
}
void spinlock_exit(volatile int *lock) {
    (*lock)--;
}

void MessageQueue::send(Message *messageP) {
    spinlock_enter(&messageInProgress);
    int oldTail = messageQueueTail;
    messageQueueTail = (messageQueueTail+1)%MSG_QUEUE_LENGTH;
    memcpy(&(messageQueue[oldTail]),messageP,sizeof(Message));
    spinlock_exit(&messageInProgress);
}

void MessageQueue::send(MessageType type, char value) {
    spinlock_enter(&messageInProgress);
    int oldTail = messageQueueTail;
    messageQueueTail = (messageQueueTail+1)%MSG_QUEUE_LENGTH;
    messageQueue[oldTail].type=type;
    messageQueue[oldTail].payload[0]=value;
    spinlock_exit(&messageInProgress);
}
void MessageQueue::send(MessageType type, float value) {
    spinlock_enter(&messageInProgress);
    int oldTail = messageQueueTail;
    messageQueueTail = (messageQueueTail+1)%MSG_QUEUE_LENGTH;
    messageQueue[oldTail].type=type;
    *((float*) (messageQueue[oldTail].payload))=value;
    spinlock_exit(&messageInProgress);
}

void MessageQueue::send(MessageType type, int value) {
    spinlock_enter(&messageInProgress);
    int oldTail = messageQueueTail;
    messageQueueTail = (messageQueueTail+1)%MSG_QUEUE_LENGTH;
    messageQueue[oldTail].type=type;
    ((int *)messageQueue[oldTail].payload)[0]=value;
    spinlock_exit(&messageInProgress);
}

Message* MessageQueue::receive() {
    if(messageQueueTail == messageQueueHead)
        return NULL;
    while(messageInProgress!=0);
    int oldHead = messageQueueHead;
    messageQueueHead = (messageQueueHead+1)%MSG_QUEUE_LENGTH;
    return &(messageQueue[oldHead]);
}

int MessageQueue::registerListener(MessageType type,Listener listener) {
    for(int i=0;i<LISTENERS_COUNT_MAX;i++) {
        if(listeners[i].type==MessageType::None) {
            listeners[i].type = type;
            listeners[i].listener = listener;
            return i;
        }
    }
    return -1;
}

int MessageQueue::unregisterListener(MessageType type,Listener listener) {
    for(int i=0;i<LISTENERS_COUNT_MAX;i++) {
        if(listeners[i].type==type && listeners[i].listener == listener) {
            listeners[i].type = MessageType::None;
            return i;
        }
    }
    return -1;
}
int MessageQueue::dispatch(Message *msg) {
    int count = 0;
    for(int i=0;i<LISTENERS_COUNT_MAX;i++) {
        if(listeners[i].type==msg->type) {
            (*(listeners[i].listener))(msg);
            count++;
        }
    }
    return count;
}

MessageQueue systemQueue;


