#include <gtest/gtest.h>
#include "../MessageQueue.h"
#include <cstdlib>
#include <cstring>


class MessageQueueTest : public ::testing::Test {
protected:
    Message testMessages[2] = {
        {MessageType::Button,"message one"},
        {MessageType::Serial, "message two"}
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
        {MessageType::Button,&listener1},
        {MessageType::Serial,&listener2},
        {MessageType::Serial,&listener1}
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
        fillBufferWithRandomC((char*)systemQueue.listeners,LISTENERS_COUNT_MAX*sizeof(ListenerEntry),12345);
        reset(5,5,5);
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
                systemQueue.send(&(testMessages[0]));
                break;
            case NO_LISTENERS:
                for(int i=0;i<LISTENERS_COUNT_MAX;i++)
                {
                    systemQueue.listeners[i].type=MessageType::None;
                }
                break;
            case BEFORE_WRAPAROUND:
                reset(3,MSG_QUEUE_LENGTH-1,0);
                break;
            case BEFORE_HEAD_WRAPAROUND:
                reset(MSG_QUEUE_LENGTH-1,3,0);
                break;
            case ONE_LISTENER:
                for(int i=0;i<LISTENERS_COUNT_MAX;i++)
                {
                    systemQueue.listeners[i].type=MessageType::None;
                }
                systemQueue.registerListener(entries[0].type,entries[0].listener);
                break;
            case TWO_LISTENERS:
                for(int i=0;i<LISTENERS_COUNT_MAX;i++)
                {
                    systemQueue.listeners[i].type=MessageType::None;
                }
                systemQueue.registerListener(entries[0].type,entries[0].listener);
                systemQueue.registerListener(entries[1].type,entries[1].listener);
                break;
            case LISTENERS_ARE_SET_UP:
                for(int i=0;i<LISTENERS_COUNT_MAX;i++)
                {
                    systemQueue.listeners[i].type=MessageType::None;
                }
                systemQueue.registerListener(entries[0].type,entries[0].listener);
                systemQueue.registerListener(entries[1].type,entries[1].listener);
                systemQueue.registerListener(entries[2].type,entries[2].listener);
                listener1_ran=false;
                listener2_ran=false;
                break;
        }
    }
};