#include <gtest/gtest.h>
#include "../MessageQueue.h"
#include <cstdlib>
#include <cstring>


class MessageQueueTest : public ::testing::Test {
protected:
    Message testMessages[2] = {
        {1,"message one"},
        {2, "message two"}
    };

    inline static volatile bool listener1_ran = false;
    inline static volatile bool listener2_ran = false;

    static void listener1(Message *msg) {
        listener1_ran = true;
    }

    static void listener2(Message *msg) {
        listener2_ran = true;
    }

    ListenerEntry entries[3] = {
        {1,&listener1,NULL},
        {2,&listener2,NULL},
        {2,&listener1,NULL}
    };

    MessageQueueTest() {
    }
    void reset(int head,int tail, int inProgress) {
        systemQueue.messageQueueHead = head;
        systemQueue.messageQueueTail = tail;
        systemQueue.messageInProgress = inProgress;
    }
    int getHead() {
        return systemQueue.messageQueueHead;
    }
    int getTail() {
        return systemQueue.messageQueueTail;
    }
    Message get(int n) {
        return systemQueue.messageQueue[n];
    }

    void SetUp() override {
        fillBufferWithRandomC((char*)systemQueue.messageQueue,MSG_QUEUE_LENGTH*sizeof(Message),12345);
        reset(5,5,5);
        systemQueue.listeners = (ListenerEntry *) 42;
    }
    void fillBufferWithRandomC(char* buffer, size_t size, unsigned int seed) {
        srand(seed);
        for (size_t i = 0; i < size; ++i) {
            buffer[i] = static_cast<char>(rand() % 256);
        }
    }

    ListenerEntry* getListeners() {
        return systemQueue.listeners;
    }
#define QUEUE_IS_EMPTY 1
#define QUEUE_WITH_ONE_MEMBER 2
#define NO_LISTENERS 3
#define BEFORE_WRAPAROUND 4
#define ONE_LISTENER 5
#define BEFORE_HEAD_WRAPAROUND 6
#define TWO_LISTENERS 7
#define LISTENERS_ARE_SET_UP 8
    void when(int state) {
        switch(state) {
            case    QUEUE_IS_EMPTY:
                reset(0,0,0);
                break;
            case    QUEUE_WITH_ONE_MEMBER:
                reset(0,0,0);
                systemQueue.add(&(testMessages[0]));
                break;
            case NO_LISTENERS:
                systemQueue.listeners = NULL;
                break;
            case BEFORE_WRAPAROUND:
                reset(3,MSG_QUEUE_LENGTH-1,0);
                break;
            case BEFORE_HEAD_WRAPAROUND:
                reset(MSG_QUEUE_LENGTH-1,3,0);
                break;
            case ONE_LISTENER:
                systemQueue.listeners=NULL;
                systemQueue.registerListener(&(entries[0]));
                break;
            case TWO_LISTENERS:
                systemQueue.listeners=NULL;
                systemQueue.registerListener(&(entries[0]));
                systemQueue.registerListener(&(entries[1]));
                break;
            case LISTENERS_ARE_SET_UP:
                systemQueue.listeners = NULL;
                systemQueue.registerListener(&(entries[0]));
                systemQueue.registerListener(&(entries[1]));
                systemQueue.registerListener(&(entries[2]));
                listener1_ran=false;
                listener2_ran=false;
                break;
        }
    }
};