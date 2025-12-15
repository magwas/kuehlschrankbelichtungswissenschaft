#ifndef MY_CONFIG_H
#define MY_CONFIG_H
#include <stdint.h>

#define PAYLOAD_LENGTH 32
#define LISTENERS_COUNT_MAX 16
#define MSG_QUEUE_LENGTH 20

#define PWM_65_1 3
#define PWM_65_2 5
#define PWM_65_3 6
#define PWM_18_1 9
#define PWM_18_2 10
#define PWM_MAINS 11
#define LAMP_RELAY A0
#define ONE_WIRE_BUS 13

#define INT_1S 7

#define COMMAND_TABLE \
    ENTRY(lamp,   Lamp,   1) \
    ENTRY(set,    Set,    2) \
    ENTRY(relay,  Relay,  1) \
    ENTRY(error,  Error,  2) \
    ENTRY(errors,  Errors,  1) \
    ENTRY(clock,  Clock,  2)

#define BUTTONS \
    ENTRY(0,'^')\
    ENTRY(1,'v')

enum class MessageType : uint8_t {
    None,
    Button,
    Serial,
    OneHz,
    Command,
    TemperatureRequest,
    Temperature,
    Console,
    Relay,
    Set,
    Lamp,
    Clock,
    Error,
    Errors,
    Housekeep
};

#define TEST_PWM 6

#endif