#define MSG_QUEUE_INTERNALS
#include "MessageQueue.h"
#include <string.h>

void MessageQueue::add(Message *messageP) {
    messageInProgress++;
    int oldTail = messageQueueTail;
    messageQueueTail = (messageQueueTail+1)%MSG_QUEUE_LENGTH;
    memcpy(&(messageQueue[oldTail]),messageP,sizeof(Message));
    messageInProgress--;
}

Message* MessageQueue::get() {
    if(messageQueueTail == messageQueueHead)
        return NULL;
    while(messageInProgress!=0);
    int oldHead = messageQueueHead;
    messageQueueHead = (messageQueueHead+1)%MSG_QUEUE_LENGTH;
    return &(messageQueue[oldHead]);
}

ListenerEntry* MessageQueue::listenersGetTail() {
    if(NULL == listeners)
        return NULL;
    ListenerEntry* current = listeners;
    while(NULL != current->next) {
        current = current->next;
    }
    return current;
}

void MessageQueue::registerListener(ListenerEntry* entry) {
    entry->next = NULL;
    ListenerEntry * tail = listenersGetTail();
    if(NULL == tail) {
        listeners = entry;
        return;
    }
    tail->next = entry;
}

void MessageQueue::unregisterListener(ListenerEntry * entry) {
    if(NULL==listeners)
        return;
    if(listeners == entry) {
        listeners = entry->next;
        return;
    }
    ListenerEntry *current = listeners;
    while(NULL != current->next) {
        if(current->next == entry) {
            current->next = entry->next;
            return;
        }
    }
}

#include <stdio.h>
void MessageQueue::dispatch(Message *msg) {
        if(NULL == listeners)
            return;
        ListenerEntry* current = listeners;
        while(true) {
            if(current->messageType == msg->type) {
                (*(current->listener))(msg);
            }
            current = current->next;
            if(NULL == current)
                return;
        }
}

MessageQueue systemQueue;


