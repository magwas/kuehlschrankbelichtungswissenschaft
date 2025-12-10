#include <Arduino.h>
#include "config.h"
#include "MessageQueue.h"

inline static void interruptForButton0() {
    systemQueue.send(MessageType::Button,(char)0);
}

inline static void interruptForButton1() {
    systemQueue.send(MessageType::Button,(char)1);
}


class Buttons {
    public:
        void begin() {
              pinMode(0,INPUT_PULLUP);
              attachInterrupt(digitalPinToInterrupt(0), interruptForButton0, FALLING);
              pinMode(1,INPUT_PULLUP);
              attachInterrupt(digitalPinToInterrupt(1), interruptForButton1, FALLING);
        }
};

extern Buttons buttons;