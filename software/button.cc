#include "button.h"

#define ENTRY(pin, value) \
    inline static void interruptForButton##pin() { \
        systemQueue.send(MessageType::Button,(char)value); \
    }
BUTTONS
#undef ENTRY

void Buttons::begin() {
#define ENTRY(pin, value) \
    pinMode(pin, INPUT_PULLUP); \
    attachInterrupt(digitalPinToInterrupt(pin), interruptForButton##pin, FALLING);

    BUTTONS
#undef ENTRY
}

Buttons buttons;
